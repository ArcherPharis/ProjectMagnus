// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitListEntry.h"
#include "Components/TextBlock.h"
#include "PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "PRAttributeSet.h"



void UUnitListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	APlayerCharacter* character = GetListItem<APlayerCharacter>();
	unitPortrait->SetBrushFromTexture(character->GetUnitPortrait());
	unitName->SetText(FText::FromString(character->GetUnitName().ToString()));
	unitSelectButton->OnReleased.AddDynamic(this, &UUnitListEntry::UnitButtonPressed);
	APCount->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetActionPoints())));
}

void UUnitListEntry::UnitButtonPressed()
{
	APlayerCharacter* character = GetListItem<APlayerCharacter>();
	if (character->CharacterCanAct())
	{
		character->onUnitDeployed.Broadcast(character);
		character->ChangeAP(-1);
	}
}

void UUnitListEntry::SetUnitEntryAP(float value)
{
	APCount->SetText(FText::FromString(FString::FromInt(value)));
}
