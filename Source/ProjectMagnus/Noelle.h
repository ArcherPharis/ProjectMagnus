// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Noelle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API ANoelle : public APlayerCharacter
{
	GENERATED_BODY()

public:
	ANoelle();

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	class UChaplainClass* chaplainClass;
	
};
