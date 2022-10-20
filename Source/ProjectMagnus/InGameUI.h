// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void GetNewWeaponInfo(class AWeapon* weapon);

	UFUNCTION()
	void UpdateHealth(float health, float maxHealth);

	UFUNCTION()
	void UpdateStamina(float health, float maxHealth);

private:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* inGameCanvas;

	UPROPERTY(meta = (BindWidget))
	class UImage* crossHair;

	UPROPERTY(meta = (BindWIdget))
	class UProgressBar* healthBar;

	UPROPERTY(meta = (BindWIdget))
	class UProgressBar* staminaBar;

	UPROPERTY(meta = (BindWidget))
	class UValueGauge* healthGauge;

	UPROPERTY(meta = (BindWidget))
	class UValueGauge* staminaGauge;

	
};
