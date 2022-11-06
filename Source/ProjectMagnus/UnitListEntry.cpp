// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitListEntry.h"
#include "Components/TextBlock.h"
#include "PlayerCharacter.h"
#include "Components/Image.h"

void UUnitListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	APlayerCharacter* character = GetListItem<APlayerCharacter>();
	unitPortrait->SetBrushFromTexture(character->GetUnitPortrait());
	unitName->SetText(FText::FromString(character->GetUnitName().ToString()));
}
