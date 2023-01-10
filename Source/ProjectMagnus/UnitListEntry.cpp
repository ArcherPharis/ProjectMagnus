// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitListEntry.h"
#include "Components/TextBlock.h"
#include "PlayerCharacter.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "MPlayerController.h"
#include "Components/Button.h"
#include "PRAttributeSet.h"
#include "StatBoxEntry.h"



void UUnitListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	APlayerCharacter* character = GetListItem<APlayerCharacter>();
	unitPortrait->SetBrushFromTexture(character->GetUnitPortrait());
	unitName->SetText(FText::FromString(character->GetUnitName().ToString()));
	unitSelectButton->OnReleased.AddDynamic(this, &UUnitListEntry::UnitButtonPressed);
	APCount->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetActionPoints())));
	character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(character->GetAttributeSet()->GetActionPointsAttribute()).AddUObject(this, &UUnitListEntry::APUpdated);
}

void UUnitListEntry::UnitButtonPressed()
{

	//when this button is pressed, we let the controller know that we did and send it this
	//character tied to this entry. the controller then shows the statbox with the updated info.


	AMPlayerController* cont = Cast<AMPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	APlayerCharacter* character = GetListItem<APlayerCharacter>();
	cont->DisplayTacticsStatBox(character);
	
	

}

void UUnitListEntry::APUpdated(const FOnAttributeChangeData& AttributeData)
{
	APCount->SetText(FText::FromString(FString::FromInt(AttributeData.NewValue)));
}

void UUnitListEntry::SetUnitEntryAP(float value)
{
	APCount->SetText(FText::FromString(FString::FromInt(value)));
}
