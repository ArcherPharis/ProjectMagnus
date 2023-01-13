// Fill out your copyright notice in the Description page of Project Settings.


#include "Irving.h"
#include "Engineer.h"

AIrving::AIrving()
{
	engineerClass = CreateDefaultSubobject<UEngineer>(TEXT("ChaplainClass"));
}

void AIrving::BeginPlay()
{
	Super::BeginPlay();
	engineerClass->GiveClassBonuses(this);
}

UBaseUnitClass* AIrving::GetBaseUnitClass()
{
	return engineerClass;
}
