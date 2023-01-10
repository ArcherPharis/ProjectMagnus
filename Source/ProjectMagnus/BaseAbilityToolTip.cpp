// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbilityToolTip.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UBaseAbilityToolTip::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBaseAbilityToolTip::SetParameters(FText skillName, FText skillDescription, UTexture2D* icon)
{
	skillNameText->SetText(skillName);
	skillDescriptionText->SetText(skillDescription);
	abilityIcon->SetBrushFromTexture(icon);

}
