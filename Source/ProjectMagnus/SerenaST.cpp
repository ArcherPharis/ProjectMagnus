// Fill out your copyright notice in the Description page of Project Settings.


#include "SerenaST.h"
#include "ShockTrooperClass.h"
#include "PRGameplayAbilityBase.h"

ASerenaST::ASerenaST()
{
	shockTrooperClass = CreateDefaultSubobject<UShockTrooperClass>(TEXT("ChaplainClass"));
}

void ASerenaST::BeginPlay()
{
	Super::BeginPlay();
	shockTrooperClass->GiveClassBonuses(this);
}

UBaseUnitClass* ASerenaST::GetBaseUnitClass()
{
	return shockTrooperClass;
}

