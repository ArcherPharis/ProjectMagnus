// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUnitClass.h"
#include "ShockTrooperClass.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UShockTrooperClass : public UBaseUnitClass
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
