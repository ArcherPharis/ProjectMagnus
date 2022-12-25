// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gear.h"
#include "TacticalGear.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API ATacticalGear : public AGear
{
	GENERATED_BODY()

public:
	void ThrowGear(float power, FVector direction);

	FGameplayTag GetEventTag() const { return TacticalDamageEventTag; }

private:
	virtual void UseGear() override;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<class AProjectile> projectileClass;

	UPROPERTY(EditAnywhere, Category = "Gear")
	FGameplayTag TacticalDamageEventTag;
	
};
