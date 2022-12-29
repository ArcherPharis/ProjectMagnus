// Fill out your copyright notice in the Description page of Project Settings.


#include "Noelle.h"
#include "ChaplainClass.h"
#include "PRGameplayAbilityBase.h"

ANoelle::ANoelle()
{
	chaplainClass = CreateDefaultSubobject<UChaplainClass>(TEXT("ChaplainClass"));
}

void ANoelle::BeginPlay()
{
	Super::BeginPlay();
	GiveAbility(chaplainClass->GetClassFieldAbility(), 6, false);
	chaplainClass->GiveClassBonuses(this);
}
