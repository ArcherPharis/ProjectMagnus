// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PRAbilitySystemBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UPRAbilitySystemBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:
	static FName GetTagNameLast(const FGameplayTag& Tag);
	static FName GetRandomNameFromTagContainer(const FGameplayTagContainer& container);
	
};
