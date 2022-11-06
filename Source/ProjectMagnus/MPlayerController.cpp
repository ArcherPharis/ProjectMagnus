// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"
#include "PlayerCharacter.h"
#include "InGameUI.h"
#include "StatComponent.h"
#include "PMGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AMPlayerController::OnPossess(APawn* newPawn)
{
	Super::OnPossess(newPawn);
	playerCharacter = Cast<APlayerCharacter>(newPawn);
	if (playerCharacter)
	{
		playerCharacter->onWeaponEquipped.AddDynamic(inGameUI, &UInGameUI::GetNewWeaponInfo);
		playerCharacter->GetStatComponent()->onHealthChange.AddDynamic(inGameUI, &UInGameUI::UpdateHealth);
		playerCharacter->GetStatComponent()->onStamChange.AddDynamic(inGameUI, &UInGameUI::UpdateStamina);
		playerCharacter->onUnitGiven.AddDynamic(inGameUI, &UInGameUI::NewUnitGiven);

	}
	else
	{
		//this would be where the special tactics mode actor logic would go
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
