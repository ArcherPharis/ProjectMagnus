// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "StatBoxEntry.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UStatBoxEntry : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void ShowStatBox(class APlayerCharacter* character);

private:
	UFUNCTION()
	void DeployCharacter();

	void CreateToolTips(APlayerCharacter* character, class UTexture2D* icon, TSubclassOf<class UBaseAbilityToolTip> widgetClass, class UImage* image, class UPRGameplayAbilityBase* ability);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* unitName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* unitClass;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* unitLevel;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* vigor;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* maxVigor;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* stamina;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* maxStamina;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* strength;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* endurance;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* agility;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* dexterity;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* currentAP;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* maxAP;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* currentEXP;

	UPROPERTY(meta = (BindWidget))
	class UButton* deployButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* unitFlairImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* fireArmName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* fireArmCurrentAmmo;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* fireArmAmmoReserved;
	UPROPERTY(meta = (BindWidget))
	UImage* firearmImage;

	UPROPERTY(meta = (BindWidget))
	UImage* fieldAbilityImage;

	UPROPERTY(meta = (BindWidget))
	UImage* classSkillOneImage;

	UPROPERTY(meta = (BindWidget))
	UImage* uniqueSkillOneImage;

	UPROPERTY(EditDefaultsOnly, Category = "Checks")
	bool isDeployableStatBox = true;

	APlayerCharacter* characterToDeploy;

	
	
};
