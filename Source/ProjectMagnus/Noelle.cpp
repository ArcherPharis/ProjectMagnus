// Fill out your copyright notice in the Description page of Project Settings.


#include "Noelle.h"
#include "ChaplainClass.h"

ANoelle::ANoelle()
{
	chaplainClass = CreateDefaultSubobject<UChaplainClass>(TEXT("ChaplainClass"));
}
