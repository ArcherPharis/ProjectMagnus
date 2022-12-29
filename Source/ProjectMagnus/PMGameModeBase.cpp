// Fill out your copyright notice in the Description page of Project Settings.


#include "PMGameModeBase.h"
#include "PlayerCharacter.h"
#include "PRAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "MPlayerController.h"
#include "BaseEnemy.h"
#include "InGameUI.h"


void APMGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void APMGameModeBase::GetAllSpawnLocations()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), spawnPoints);
	
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
		//show level up stuff here. hide battle UI, etc.
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

		killedEnemyUnits.RemoveAt(0);

	}
	else
	{
		AMPlayerController* mCont = Cast<AMPlayerController>(controller);
		CharacterToReturnTo->AfterUnitDeath();
		mCont->SetFieldCanvas();
	}


}

void APMGameModeBase::SpawnInitialUnits(UInGameUI* aUI)
{
	GetAllSpawnLocations();

	for (TSubclassOf<APlayerCharacter> character : deployablePlayerUnits)
	{
		APlayerCharacter* initCharacter =  GetWorld()->SpawnActor<APlayerCharacter>(character);
		currentPlayerUnits.Add(initCharacter);
		aUI->NewUnitGiven(initCharacter);
		
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

void APMGameModeBase::AddDownedUnits(ACharacter_Base* downedUnit)
{
	downedPlayerUnits.Add(downedUnit);

	
}

void APMGameModeBase::AddKilledUnits(ABaseEnemy* killedUnit)
{
	killedEnemyUnits.Add(killedUnit);

	if (GetWorldTimerManager().IsTimerActive(killedOrDownedUnitHandle)) return;

	GetWorldTimerManager().SetTimer(killedOrDownedUnitHandle, this, &APMGameModeBase::ShowDownedDeadUnits, 1.f, false);
}
