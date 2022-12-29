// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Weapon.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "ValueGauge.h"
#include "PlayerCharacter.h"
#include "BaseEnemy.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "PRAttributeSet.h"
#include "APValue.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	firearmForecastImage->SetVisibility(ESlateVisibility::Hidden);
	forecastHoriBox->SetVisibility(ESlateVisibility::Hidden);
	stopFiringOverlayButton->bIsEnabled = false;
	stopFiringOverlayButton->OnPressed.AddDynamic(this, &UInGameUI::StopFiring);
}

void UInGameUI::ShowVigorText(float oldValue, float newValue)
{

	oldHealthLevelUpText->SetVisibility(ESlateVisibility::Visible);
	newHealthLevelUpText->SetVisibility(ESlateVisibility::Visible);
	oldHealthLevelUpText->SetText(FText::FromString(FString::FromInt(oldValue)));
	newHealthLevelUpText->SetText(FText::FromString(FString::FromInt(newValue)));
}

void UInGameUI::ShowLevelUpText(float oldValue, float newValue, UTextBlock* oldText, UTextBlock* newText)
{
	oldText->SetVisibility(ESlateVisibility::Visible);
	newText->SetVisibility(ESlateVisibility::Visible);
	oldText->SetText(FText::FromString(FString::FromInt(oldValue)));
	newText->SetText(FText::FromString(FString::FromInt(newValue)));


}

void UInGameUI::GetNewWeaponInfo(AWeapon* weapon)
{
	weapon->onWeaponUse.AddDynamic(this, &UInGameUI::UpdateAmmoCount);
	UTexture2D* crossHairImage = weapon->GetWeaponCrosshair();
	UTexture2D* wIcon = weapon->GetWeaponIcon();
	if (crossHairImage && weaponIcon)
	{
		crossHair->SetBrushFromTexture(crossHairImage);
		crossHair->SetVisibility(ESlateVisibility::Visible);
		weaponIcon->SetBrushFromTexture(wIcon);
		weaponIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		crossHair->SetVisibility(ESlateVisibility::Hidden);
	}
	FString ammoText = FString::FromInt(weapon->GetCurrentAmmo());
	ammoCount->SetText(FText::FromString(ammoText));
	FString ammoReserves = FString::FromInt(weapon->GetAmmoReserves());
	ammoReservesText->SetText(FText::FromString(ammoReserves));
	
}

void UInGameUI::ShowLevelUpScreen(APlayerCharacter* unit, float oldV, float oldS, float oldStr, float oldE, float oldA, float oldD)
{
	UISwitcher->SetActiveWidget(levelUpCanvas);
	unitImageLevelUp->SetBrushFromTexture(unit->GetUnitPortrait());
	LevelUpEvent(oldV, unit->GetAttributeSet()->GetMaxHealth(),
		oldS, unit->GetAttributeSet()->GetMaxStamina(),
		oldStr, unit->GetAttributeSet()->GetStrength(),
		oldE, unit->GetAttributeSet()->GetEndurance(),
		oldA, unit->GetAttributeSet()->GetAgility(),
		oldD, unit->GetAttributeSet()->GetDexterity(),
		unit->GetAttributeSet()->GetActionPoints(), unit->GetAttributeSet()->GetActionPoints(),
		unit->GetAttributeSet()->GetLevel() - 1, unit->GetAttributeSet()->GetLevel());



}

void UInGameUI::DisplayTargetStats(ACharacter_Base* target, float health, float maxHealth)
{
	targetHealthBar->SetPercent(health / maxHealth);
	targetCurrentHealthText->SetText(FText::FromString(FString::FromInt(health)));
	targetMaxHealthText->SetText(FText::FromString(FString::FromInt(maxHealth)));
	targetName->SetText(FText::FromName(target->GetUnitName()));


}

void UInGameUI::DisplayEnemyTargetStats(ABaseEnemy* target, float health, float maxHealth)
{
	targetHealthBar->SetPercent(health / maxHealth);
	targetCurrentHealthText->SetText(FText::FromString(FString::FromInt(health)));
	targetMaxHealthText->SetText(FText::FromString(FString::FromInt(maxHealth)));
	targetName->SetText(FText::FromName(target->GetUnitName()));
}

void UInGameUI::DisplayTargetInfo(bool displayOrNot)
{
	if (displayOrNot)
	{
		targetHealthBar->SetVisibility(ESlateVisibility::Visible);
		targetOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		targetHealthBar->SetVisibility(ESlateVisibility::Hidden);
		targetOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInGameUI::UpdateHealth(float health, float maxHealth)
{
	//healthBar->SetPercent(health / maxHealth);
	
	healthGauge->SetValue(health, maxHealth);
	healthGauge->SetHealthRange(maxHealth);
	
	
}

void UInGameUI::UpdateStamina(float stam, float maxStam)
{
	//staminaBar->SetPercent(health / maxHealth);
	
	staminaGauge->SetValue(stam, maxStam);
	staminaGauge->SetStamRange(maxStam);
	

}

void UInGameUI::UpdateExperience(float exp, float mExp)
{
	experienceGauge->SetValue(exp, mExp);
}

void UInGameUI::UpdateAmmoCount(int ammo, int ammoReserves)
{
	FString ammoText = FString::FromInt(ammo);
	ammoCount->SetText(FText::FromString(ammoText));
	FString ammoReservesT = FString::FromInt(ammoReserves);
	ammoReservesText->SetText(FText::FromString(ammoReservesT));
}

void UInGameUI::NewUnitGiven(APlayerCharacter* unit)
{
	unitList->AddItem(unit);
	unit->onUnitDeployed.AddDynamic(this, &UInGameUI::PossessNewUnit);

}

void UInGameUI::SetValueSegments(float maxSegments)
{
	APGauge->SetMaxValue(maxSegments);
}

void UInGameUI::SetAPText(float newValue)
{
	APCount->SetText(FText::FromString(FString::FromInt(newValue)));
}

void UInGameUI::UpdateRanges(float maxHealth, float maxStam, float exp, float mExp)
{
	healthGauge->SetHealthRange(maxHealth);
	staminaGauge->SetStamRange(maxStam);
	experienceGauge->SetValue(exp, mExp);
}

void UInGameUI::PossessNewUnit(APlayerCharacter* newUnit)
{
	AController* controller = UGameplayStatics::GetPlayerController(this, 0);
	controller->Possess(newUnit);
	UISwitcher->SetActiveWidget(inGameCanvas);

	//the character also needs to tell the ui it would like to send its data over, hence
	//the lack of icons and recticle
	
}


void UInGameUI::SetForecast(int STK, int ammoLeft)
{
	firearmForecastImage->SetVisibility(ESlateVisibility::Visible);
	forecastHoriBox->SetVisibility(ESlateVisibility::Visible);
	shotsToKillText->SetText(FText::FromString(FString::FromInt(STK)));
	ammoLeftText->SetText(FText::FromString(FString::FromInt(ammoLeft)));
}

void UInGameUI::ClearForecast()
{
	firearmForecastImage->SetVisibility(ESlateVisibility::Hidden);
	forecastHoriBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameUI::UnhideButton()
{

	
	if (stopFiringOverlayButton->bIsEnabled)
	{
		stopFiringOverlayButton->bIsEnabled = false;
		return;
	}

	stopFiringOverlayButton->bIsEnabled = true;


}

void UInGameUI::StopFiring()
{
	
	onButtonPressed.Broadcast();
	stopFiringOverlayButton->bIsEnabled = false;
}

void UInGameUI::SetTipText(FString text)
{
	tipText->SetText(FText::FromString(text));
}

void UInGameUI::EnableFieldCanvas()
{
	UISwitcher->SetActiveWidget(inGameCanvas);
	HideLevelUpTextPair(oldStaminaText, newStaminaText);
	HideLevelUpTextPair(oldStrengthText, newStrengthText);
	HideLevelUpTextPair(oldEnduranceText, newEnduranceText);
	HideLevelUpTextPair(oldAgilityText, newAgilityText);
	HideLevelUpTextPair(oldDexText, newDexText);
	HideLevelUpTextPair(oldAPText, newAPText);
	HideLevelUpTextPair(oldLevelText, newLevelText);
	HideLevelUpTextPair(oldHealthLevelUpText, newHealthLevelUpText);


	
}

void UInGameUI::HideLevelUpTextPair(UTextBlock* one, UTextBlock* two)
{
	one->SetVisibility(ESlateVisibility::Hidden);
	two->SetVisibility(ESlateVisibility::Hidden);

}
