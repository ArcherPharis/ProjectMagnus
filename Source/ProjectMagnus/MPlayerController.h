// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* newPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<class UInGameUI> inGameUIClass;

	UInGameUI* inGameUI;

	class APlayerCharacter* playerCharacter;
	
};
