// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"
#include "PlayerCharacter.h"
#include "Character_Base.h"
#include "InGameUI.h"
#include "Weapon.h"
#include "StatComponent.h"
#include "PMGameModeBase.h"
#include "PRAttributeSet.h"
#include "Kismet/GameplayStatics.h"

void AMPlayerController::OnPossess(APawn* newPawn)
{
	Super::OnPossess(newPawn);
	
	playerCharacter = Cast<APlayerCharacter>(newPawn);
	if (playerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("You possessed the player actor!"));
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		SetViewTargetWithBlend(playerCharacter, 1.5f);
		OnPossessEffect();
		playerCharacter->onUnitGiven.AddDynamic(inGameUI, &UInGameUI::NewUnitGiven);
		playerCharacter->onWeaponEquipped.AddDynamic(inGameUI, &UInGameUI::GetNewWeaponInfo);
		playerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(playerCharacter->GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &AMPlayerController::HealthUpdated);
		playerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(playerCharacter->GetAttributeSet()->GetStaminaAttribute()).AddUObject(this, &AMPlayerController::StaminaUpdated);
		playerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(playerCharacter->GetAttributeSet()->GetActionPointsAttribute()).AddUObject(this, &AMPlayerController::APUpdated);
		playerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(playerCharacter->GetAttributeSet()->GetExperiencePointsAttribute()).AddUObject(this, &AMPlayerController::ExperienceUpdate);
		playerCharacter->onShowUnitMenu.AddDynamic(inGameUI, &UInGameUI::ShowUnitMenu);
		playerCharacter->onAPGauge.AddDynamic(inGameUI, &UInGameUI::SetAPText);
		playerCharacter->onUpdateHealthStamRange.AddDynamic(inGameUI, &UInGameUI::UpdateRanges);
		playerCharacter->onDisplayTip.AddDynamic(inGameUI, &UInGameUI::SetTipText);
		playerCharacter->onInitiateLevelUp.AddDynamic(inGameUI, &UInGameUI::ShowLevelUpScreen);
		playerCharacter->GetCurrentWeapon()->onBeginAttackEvent.AddDynamic(inGameUI, &UInGameUI::UnhideButton);
		playerCharacter->onUnitTarget.AddDynamic(inGameUI, &UInGameUI::DisplayTargetStats);
		playerCharacter->onEnemyUnitTarget.AddDynamic(inGameUI, &UInGameUI::DisplayEnemyTargetStats);
		playerCharacter->onDisplayTargetInfo.AddDynamic(inGameUI, &UInGameUI::DisplayTargetInfo);
		//playerCharacter->GetCurrentWeapon()->onEndAttackEvent.AddDynamic(playerCharacter, &APlayerCharacter::GunAttackEventEnd);
		inGameUI->onButtonPressed.AddDynamic(this, &AMPlayerController::FiringEventStop);
		playerCharacter->OnDeployed();
		
	
	}
	else
	{
		SetViewTargetWithBlend(newPawn, 1.5f);
		UE_LOG(LogTemp, Warning, TEXT("You possessed the some other actor!"));
	}

}

void AMPlayerController::BeginPlay()
{
	inGameUI = CreateWidget<UInGameUI>(this, inGameUIClass);
	inGameUI->AddToViewport();
	gameMode = Cast<APMGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->SpawnInitialUnits(inGameUI); //I think eventually, the game is gonna start by possessing a special
	//actor just for tactics mode selection rather than any player.

}

void AMPlayerController::SetFieldCanvas()
{
	inGameUI->EnableFieldCanvas();
}

void AMPlayerController::HealthUpdated(const FOnAttributeChangeData& AttributeData)
{
	if(playerCharacter)
	inGameUI->UpdateHealth(AttributeData.NewValue, playerCharacter->GetAttributeSet()->GetMaxHealth());
}

void AMPlayerController::StaminaUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateStamina(AttributeData.NewValue, playerCharacter->GetAttributeSet()->GetMaxStamina());
}

void AMPlayerController::APUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->SetAPText(AttributeData.NewValue);
}

void AMPlayerController::ExperienceUpdate(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateExperience(AttributeData.NewValue, playerCharacter->GetAttributeSet()->GetMaxExperiencePoints());
}

void AMPlayerController::FiringEventStop()
{
	if (playerCharacter->GetCurrentWeapon()->GetInAttackEvent())
	{
		if (playerCharacter->GetCurrentWeapon()->GetPlayerWantsToStopFiring() == false)
		{
			playerCharacter->GetCurrentWeapon()->SetPlayerWantsToStopFiring(true);
		}
	}
	
}

