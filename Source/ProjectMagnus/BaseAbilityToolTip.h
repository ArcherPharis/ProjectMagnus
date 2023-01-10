// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseAbilityToolTip.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UBaseAbilityToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetParameters(FText skillName, FText skillDescription, UTexture2D* icon);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* skillNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* skillDescriptionText;

	UPROPERTY(meta = (BindWidget))
	class UImage* abilityIcon;
	
};
