// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Irving.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API AIrving : public APlayerCharacter
{

	GENERATED_BODY()
public:



	AIrving();

	virtual void BeginPlay() override;

private:

	virtual class UBaseUnitClass* GetBaseUnitClass() override;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	class UEngineer* engineerClass;
	
};
