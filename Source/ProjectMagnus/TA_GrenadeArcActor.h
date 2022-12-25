// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TA_GrenadeArcActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API ATA_GrenadeArcActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	ATA_GrenadeArcActor();

private:
	virtual void ConfirmTargetingAndContinue() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Arc")
	float throwDistance = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Arc")
	USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly, Category = "Arc")
	class USphereComponent* blastRadius;
	
};
