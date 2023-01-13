// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "SerenaST.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API ASerenaST : public APlayerCharacter
{
	GENERATED_BODY()

public:
	ASerenaST();

	virtual void BeginPlay() override;

private:

	virtual class UBaseUnitClass* GetBaseUnitClass() override;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	class UShockTrooperClass* shockTrooperClass;
	
};
