// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"
#include "PlayerCharacter.h"
#include "InGameUI.h"
#include "StatComponent.h"

void AMPlayerController::OnPossess(APawn* newPawn)
{
	Super::OnPossess(newPawn);
	playerCharacter = Cast<APlayerCharacter>(newPawn);
	if (playerCharacter)
	{
		inGameUI = CreateWidget<UInGameUI>(this, inGameUIClass);
		inGameUI->AddToViewport();
		playerCharacter->onWeaponEquipped.AddDynamic(inGameUI, &UInGameUI::GetNewWeaponInfo);
		playerCharacter->GetStatComponent()->onHealthChange.AddDynamic(inGameUI, &UInGameUI::UpdateHealth);
		playerCharacter->GetStatComponent()->onStamChange.AddDynamic(inGameUI, &UInGameUI::UpdateStamina);


	}

}
