// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBoxEntry.h"
#include "Components/TextBlock.h"
#include "PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "PRAttributeSet.h"
#include "BaseUnitClass.h"
#include "BaseAbilityToolTip.h"
#include "Weapon.h"
#include "PRGameplayAbilityBase.h"


void UStatBoxEntry::NativeConstruct()
{
	Super::NativeConstruct();
	deployButton->OnPressed.AddDynamic(this, &UStatBoxEntry::DeployCharacter);
}

void UStatBoxEntry::ShowStatBox(APlayerCharacter* character)
{
	characterToDeploy = character;

	if (isDeployableStatBox)
	{
		if (GetVisibility() == ESlateVisibility::Visible)
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
		else if (GetVisibility() == ESlateVisibility::Hidden)
		{
			SetVisibility(ESlateVisibility::Visible);
		}

		if (characterToDeploy->GetAttributeSet()->GetActionPoints() == 0 || characterToDeploy->GetAttributeSet()->GetHealth() == 0)
		{
			deployButton->SetIsEnabled(false);
		}
		else if (characterToDeploy->GetAttributeSet()->GetActionPoints() > 0)
		{
			deployButton->SetIsEnabled(true);
		}
	}

	unitName->SetText(FText::FromString(character->GetUnitName().ToString()));
	unitClass->SetText(FText::FromString(character->GetPlayerUnitClassName().ToString()));
	unitLevel->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetLevel())));
	vigor->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetHealth())));
	maxVigor->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetMaxHealth())));
	stamina->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetStamina())));
	maxStamina->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetMaxStamina())));
	strength->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetStrength())));
	endurance->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetEndurance())));
	agility->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetAgility())));
	dexterity->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetDexterity())));
	currentAP->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetActionPoints())));
	maxAP->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetMaxActionPoints())));
	currentEXP->SetText(FText::FromString(FString::FromInt(character->GetAttributeSet()->GetExperiencePoints())));
	fireArmCurrentAmmo->SetText(FText::FromString(FString::FromInt(character->GetCurrentWeapon()->GetCurrentAmmo())));
	fireArmAmmoReserved->SetText(FText::FromString(FString::FromInt(character->GetCurrentWeapon()->GetAmmoReserves())));
	firearmImage->SetBrushFromTexture(character->GetCurrentWeapon()->GetWeaponIcon());
	fireArmName->SetText(FText::FromString((character->GetCurrentWeapon()->GetWeaponName().ToString())));
	

	CreateToolTips(character);

}

void UStatBoxEntry::DeployCharacter()
{
	if (characterToDeploy->CharacterCanAct())
	{
		characterToDeploy->onUnitDeployed.Broadcast(characterToDeploy);
		characterToDeploy->ChangeAP(-1);
	}




}

void UStatBoxEntry::CreateToolTips(APlayerCharacter* character)
{
	fieldAbilityImage->SetBrushFromTexture(character->GetBaseUnitClass()->GetClassFieldAbility()->GetIcon());
	UBaseAbilityToolTip* tooltip = CreateWidget<UBaseAbilityToolTip>(this, character->GetBaseUnitClass()->GetClassFieldAbility()->GetToolTipWidget());
	character->GetBaseUnitClass()->GetClassFieldAbility()->SetToolTip(tooltip);
	fieldAbilityImage->SetToolTip(character->GetBaseUnitClass()->GetClassFieldAbility()->GetToolTip());


	classSkillOneImage->SetBrushFromTexture(character->GetBaseUnitClass()->GetClassAbilityOne()->GetIcon());
	UBaseAbilityToolTip* tt1 = CreateWidget<UBaseAbilityToolTip>(this, character->GetBaseUnitClass()->GetClassAbilityOne()->GetToolTipWidget());
	character->GetBaseUnitClass()->GetClassAbilityOne()->SetToolTip(tt1);
	classSkillOneImage->SetToolTip(character->GetBaseUnitClass()->GetClassAbilityOne()->GetToolTip());
}
