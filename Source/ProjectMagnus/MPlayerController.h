// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayEffectTypes.h"
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

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "ControllerEvent")
	void OnPossessEffect();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<class UInGameUI> inGameUIClass;

	void HealthUpdated(const FOnAttributeChangeData& AttributeData);

	void StaminaUpdated(const FOnAttributeChangeData& AttributeData);

	void APUpdated(const FOnAttributeChangeData& AttributeData);

	UInGameUI* inGameUI;

	class APlayerCharacter* playerCharacter;

	class APMGameModeBase* gameMode;
	
};
