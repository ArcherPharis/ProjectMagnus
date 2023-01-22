// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "HitDetectionComponent.h"

AMeleeWeapon::AMeleeWeapon()
{
	HitDetectionComp = CreateDefaultSubobject<UHitDetectionComponent>("HitDetectionComp");
	HitDetectionComp->SetupAttachment(GetWeaponMesh());
}
