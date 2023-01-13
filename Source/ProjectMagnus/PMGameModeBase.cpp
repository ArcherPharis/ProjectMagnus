// Fill out your copyright notice in the Description page of Project Settings.


#include "PMGameModeBase.h"
#include "PlayerCharacter.h"
#include "PRAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "PRAIControllerBase.h"
#include "MPlayerController.h"
#include "Weapon.h"
#include "BaseEnemy.h"
#include "InGameUI.h"
#include "TacticsPawn.h"


void APMGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void APMGameModeBase::GetAllSpawnLocations()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), spawnPoints);
	TArray<AActor*> potEnems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), potEnems);

	for (AActor* a : potEnems)
	{
		ABaseEnemy* enemy = Cast<ABaseEnemy>(a);
		currentEnemyUnits.Add(enemy);
	}


	TacticsPawn = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATacticsPawn::StaticClass()));
	
}

void APMGameModeBase::PossessTacticsActor()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	controller->Possess(TacticsPawn);
	ui->EnableTacticsCanvas();
	TogglePlayerLogic(false);
	currentlyDeployedPlayerUnit->AIControllerRepossess();
	currentlyDeployedPlayerUnit = nullptr;
}

void APMGameModeBase::CycleThroughEnemyUnitTurns(ABaseEnemy* enemyDoneWithTurn)
{
	APlayerController* cont = UGameplayStatics::GetPlayerController(this, 0);
	enemyDoneWithTurn->GetAIController()->SetStandingBehaviorTree();
	enemyDoneWithTurn->SetLogicEnabled(false);
	movingEnemyUnits.Remove(enemyDoneWithTurn);

	//here we check if there are any dead ally units, if there is, play events and stuff for
	//them before returning back here.

	if (downedPlayerUnits.Num() > 0)
	{
		
		cont->SetViewTargetWithBlend(downedPlayerUnits[0]);
		downedPlayerUnits.RemoveAt(0);
		FTimerHandle playerHandle;
		if (movingEnemyUnits.Num() > 0)
		{
			GetWorldTimerManager().SetTimer(playerHandle, this, &APMGameModeBase::BeginEnemyTurn, 3.5f);
		}
		else
		{
			cont->SetInputMode(FInputModeUIOnly());
			cont->SetViewTargetWithBlend(TacticsPawn, 0.5f);
			ui->ShowHUD();
			EndEnemyTurn();
			currentPhase = Phase::PlayerPhase;
		}
		return;
	}

	if (movingEnemyUnits.Num() > 0)
	{
		cont->SetViewTargetWithBlend(TacticsPawn, 1.5f);
		GetWorldTimerManager().SetTimer(toNextEnemyTurnHandle, this, &APMGameModeBase::GoToNextEnemy, 3.5f);
	}
	else
	{
		cont->SetInputMode(FInputModeUIOnly());
		cont->SetViewTargetWithBlend(TacticsPawn, 0.5f);
		ui->HideTacticsStatBox();
		ui->ShowHUD();
		EndEnemyTurn();
		
		currentPhase = Phase::PlayerPhase;
	}
}

void APMGameModeBase::BeginEnemyTurn()
{


	if (movingEnemyUnits.Num() > 0)
	{

		currentPhase = Phase::EnemyPhase;
		ui->HideHUD();

		for (APlayerCharacter* playa : currentPlayerUnits)
		{
			playa->AIControllerRepossess();
		}

		for (APlayerCharacter* p : currentPlayerUnits)
		{
			if (p->GetAttributeSet()->GetActionPoints() < p->GetAttributeSet()->GetMaxActionPoints())
				p->GetAttributeSet()->SetActionPoints(p->GetAttributeSet()->GetActionPoints() + 1);
		}

		APlayerController* cont = UGameplayStatics::GetPlayerController(this, 0);
		TogglePlayerLogic(true);
		cont->SetInputMode(FInputModeUIOnly());
		cont->SetViewTargetWithBlend(movingEnemyUnits[0], 0.5f);
		movingEnemyUnits[0]->BeginEnemyTurn();
	}
	else
	{
		for (APlayerCharacter* p : currentPlayerUnits)
		{
			if (p->GetAttributeSet()->GetActionPoints() < p->GetAttributeSet()->GetMaxActionPoints() && p->GetAttributeSet()->GetHealth() > 0)
				p->GetAttributeSet()->SetActionPoints(p->GetAttributeSet()->GetActionPoints() + 1);
		}
	}


}

void APMGameModeBase::ShowDownedDeadUnits()
{
	APlayerController* controller =  UGameplayStatics::GetPlayerController(this, 0);

	if (CharacterToReturnTo && CharacterToReturnTo->GetAttributeSet()->GetExperiencePoints() == 100)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has 100 exp, reset their exp!"));

		GetWorldTimerManager().ClearTimer(killedOrDownedUnitHandle);
		CharacterToReturnTo->GetAttributeSet()->SetExperiencePoints(0);
		CharacterToReturnTo->LevelUp();
		GetWorldTimerManager().SetTimer(killedOrDownedUnitHandle, this, &APMGameModeBase::ShowDownedDeadUnits, 8.f, false);
		return;

	}


	if (killedEnemyUnits.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(killedOrDownedUnitHandle, this, &APMGameModeBase::ShowDownedDeadUnits, 8.f, false);
		APawn* DeathPawn = GetWorld()->SpawnActor<APawn>(killedEnemyUnits[0]->GetDeathPawnClass(), killedEnemyUnits[0]->GetTransform());
		APlayerController* cont = UGameplayStatics::GetPlayerController(this, 0);
		cont->SetViewTargetWithBlend(DeathPawn, 0.5f);
		killedEnemyUnits[0]->SetActorHiddenInGame(true);
		killedEnemyUnits[0]->AwardKillerWithEXP();
		CharacterToReturnTo = Cast<APlayerCharacter>(killedEnemyUnits[0]->GetKiller());

		ABaseEnemy* ene = killedEnemyUnits[0];
		RemoveEnemyFromList(ene);
		killedEnemyUnits.RemoveAt(0);
		ene->Destroy();


	}
	else
	{

		if(IsPlayerPhase())
		{ 
			AMPlayerController* mCont = Cast<AMPlayerController>(controller);
			CharacterToReturnTo->AfterUnitDeath();
			mCont->SetFieldCanvas();
		}
		else if (IsEnemyPhase())
		{
			BeginEnemyTurn();
		}
	}
	

}

void APMGameModeBase::GoToNextEnemy()
{
	BeginEnemyTurn();

}

void APMGameModeBase::EndEnemyTurn()
{

	TogglePlayerLogic(false);



	for (APlayerCharacter* p : currentPlayerUnits)
	{
		p->GetAttributeSet()->SetStamina(p->GetAttributeSet()->GetMaxStamina());
		p->GetCurrentWeapon()->AddToAmmoReserves(p->GetCurrentWeapon()->GetMaxAmmo());

		if(p->GetAttributeSet()->GetActionPoints() < p->GetAttributeSet()->GetMaxActionPoints())
		p->GetAttributeSet()->SetActionPoints(p->GetAttributeSet()->GetActionPoints() + 1);
	}
}


bool APMGameModeBase::IsPlayerPhase()
{
	if (currentPhase == Phase::PlayerPhase)
	{
		return true;
	}
	return false;
}

bool APMGameModeBase::IsEnemyPhase()
{
	if (currentPhase == Phase::EnemyPhase)
	{
		return true;
	}
	return false;
}

void APMGameModeBase::SpawnInitialUnits(UInGameUI* aUI)
{
	GetAllSpawnLocations();

	for (TSubclassOf<APlayerCharacter> character : deployablePlayerUnits)
	{
		APlayerCharacter* initCharacter =  GetWorld()->SpawnActor<APlayerCharacter>(character);
		currentPlayerUnits.Add(initCharacter);
		aUI->NewUnitGiven(initCharacter);
		ui = aUI;
		
	}
	
	for (APlayerCharacter* player : currentPlayerUnits)
	{
		player->GiveEquipment();

	}

	if (numberOfUnitsAllowedForChapter == spawnPoints.Num())
	{
		if (currentPlayerUnits.Num() > 0 && currentPlayerUnits.Num() <= spawnPoints.Num())
		{
			for (int i = 0; i < currentPlayerUnits.Num(); i++)
			{
				currentPlayerUnits[i]->SetActorLocation(spawnPoints[i]->GetActorLocation());
				currentPlayerUnits[i]->SetActorRotation(spawnPoints[i]->GetActorRotation());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hey! You either have no deployable units or more units than spawn points!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hey! You don't have the required amount of spawn points!"));
	}
	//todo, we also put up each initial ui on the list
}

void APMGameModeBase::SetCurrentDeployedPlayerUnit(APlayerCharacter* deployed)
{
	currentlyDeployedPlayerUnit = deployed;
}

void APMGameModeBase::AddDownedUnits(ACharacter_Base* downedUnit)
{
	downedPlayerUnits.Add(downedUnit);

	
}

void APMGameModeBase::AddDeadEnemyUnit(ABaseEnemy* killedUnit)
{
	killedEnemyUnits.Add(killedUnit);
}

void APMGameModeBase::AddKilledUnits(ABaseEnemy* killedUnit)
{
	killedEnemyUnits.Add(killedUnit);

	if (GetWorldTimerManager().IsTimerActive(killedOrDownedUnitHandle)) return;

	GetWorldTimerManager().SetTimer(killedOrDownedUnitHandle, this, &APMGameModeBase::ShowDownedDeadUnits, 1.f, false);
}

void APMGameModeBase::ReturnToTacticsPawn()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);

	//if (currentEnemyUnits.Num() == 0)
	//{
	//	ui->DisableEnemyTurnButton();
	//}

	controller->SetInputMode(FInputModeUIOnly());
	controller->bShowMouseCursor = true;
	UGameplayStatics::SetGamePaused(this, false);
	ui->HideTacticsStatBox();
	controller->SetViewTargetWithBlend(TacticsPawn, 0.5f);
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &APMGameModeBase::PossessTacticsActor, 0.5f);
	ToggleEnemyLogic(false);
	TogglePlayerLogic(false);
	currentlyDeployedPlayerUnit->ToggleUseControlRotationYaw(true);

}

void APMGameModeBase::ToggleEnemyLogic(bool bStopLogic)
{

	if (currentEnemyUnits.Num() > 0)
	{
		for (ABaseEnemy* enemy : currentEnemyUnits)
		{
			enemy->SetLogicEnabled(bStopLogic);
		}
	}
}

void APMGameModeBase::TogglePlayerLogic(bool bStopLogic)
{
	if (currentPlayerUnits.Num() > 0)
	{
		for (APlayerCharacter* playa : currentPlayerUnits)
		{
			playa->SetLogicEnabled(bStopLogic);
		}
	}
}

void APMGameModeBase::RemoveEnemyFromList(ABaseEnemy* enemyToRemove)
{
	currentEnemyUnits.Remove(enemyToRemove);
}

void APMGameModeBase::PopulateMovableUnits()
{
	for (ABaseEnemy* e : currentEnemyUnits)
	{
		if (e->ActorHasTag(TEXT("MovingEnemyUnit")) && e->GetAttributeSet()->GetHealth() != 0)
		{
			movingEnemyUnits.Add(e);
		}
	}
}
