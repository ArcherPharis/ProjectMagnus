// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TA_FayeRadius.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API ATA_FayeRadius : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	ATA_FayeRadius();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	virtual void ConfirmTargeting() override;
	virtual void ConfirmTargetingAndContinue() override;

	UPROPERTY(EditAnywhere, Category = "FayeRadius")
	USceneComponent* rootComp;

	UPROPERTY(EditAnywhere, Category = "FayeRadius")
	class USphereComponent* radius;
	
};
