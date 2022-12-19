// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EPRAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	UseItem,
	Reload
};
