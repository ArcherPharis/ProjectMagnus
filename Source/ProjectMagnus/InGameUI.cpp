// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Weapon.h"
#include "Components/Image.h"

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInGameUI::GetNewWeaponInfo(AWeapon* weapon)
{
	UTexture2D* crossHairImage = weapon->GetWeaponCrosshair();
	UE_LOG(LogTemp, Warning, TEXT("InGameUI recieved the broadcast, must be execution flow"));
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
