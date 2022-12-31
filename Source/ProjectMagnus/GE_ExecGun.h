// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GE_ExecGun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UGE_ExecGun : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGE_ExecGun();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecParams, FGameplayEffectCustomExecutionOutput& ExecOutputs) const;
	
};
