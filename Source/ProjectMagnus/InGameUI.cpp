// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Weapon.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include"ValueGauge.h"

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInGameUI::GetNewWeaponInfo(AWeapon* weapon)
{
	UTexture2D* crossHairImage = weapon->GetWeaponCrosshair();
	if (crossHairImage)
	{
		crossHair->SetBrushFromTexture(crossHairImage);
		crossHair->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		crossHair->SetVisibility(ESlateVisibility::Hidden);
	}
	

}

void UInGameUI::UpdateHealth(float health, float maxHealth)
{
	healthBar->SetPercent(health / maxHealth);
	healthGauge->SetValue(health, maxHealth);
}

void UInGameUI::UpdateStamina(float health, float maxHealth)
{
	staminaBar->SetPercent(health / maxHealth);
	staminaGauge->SetValue(health, maxHealth);

}
