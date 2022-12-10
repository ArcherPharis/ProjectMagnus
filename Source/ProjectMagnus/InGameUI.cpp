// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Weapon.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include"ValueGauge.h"
#include "PlayerCharacter.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
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
	
}

void UInGameUI::UpdateHealth(float health, float maxHealth)
{
	//healthBar->SetPercent(health / maxHealth);
	healthGauge->SetValue(health, maxHealth);
}

void UInGameUI::UpdateStamina(float health, float maxHealth)
{
	//staminaBar->SetPercent(health / maxHealth);
	staminaGauge->SetValue(health, maxHealth);

}

void UInGameUI::UpdateAmmoCount(int ammo)
{
	FString ammoText = FString::FromInt(ammo);
	ammoCount->SetText(FText::FromString(ammoText));
}

void UInGameUI::NewUnitGiven(APlayerCharacter* unit)
{
	unitList->AddItem(unit);
	unit->onUnitDeployed.AddDynamic(this, &UInGameUI::PossessNewUnit);

}

void UInGameUI::PossessNewUnit(APlayerCharacter* newUnit)
{
	AController* controller = UGameplayStatics::GetPlayerController(this, 0);
	controller->Possess(newUnit);
	UISwitcher->SetActiveWidget(inGameCanvas);

	//the character also needs to tell the ui it would like to send its data over, hence
	//the lack of icons and recticle
	
}
