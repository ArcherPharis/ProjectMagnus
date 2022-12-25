// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Weapon.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "ValueGauge.h"
#include "PlayerCharacter.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "APValue.h"
#include "Components/Button.h"

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	firearmForecastImage->SetVisibility(ESlateVisibility::Hidden);
	forecastHoriBox->SetVisibility(ESlateVisibility::Hidden);
	stopFiringOverlayButton->bIsEnabled = false;
	stopFiringOverlayButton->OnPressed.AddDynamic(this, &UInGameUI::StopFiring);
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

void UInGameUI::UpdateHealth(float health, float maxHealth)
{
	//healthBar->SetPercent(health / maxHealth);
	
	healthGauge->SetValue(health, maxHealth);
	
}

void UInGameUI::UpdateStamina(float stam, float maxStam)
{
	//staminaBar->SetPercent(health / maxHealth);
	
	staminaGauge->SetValue(stam, maxStam);
	

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

void UInGameUI::UpdateRanges(float maxHealth, float maxStam)
{
	healthGauge->SetHealthRange(maxHealth);
	staminaGauge->SetStamRange(maxStam);
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
