// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRGameplayAbilityBase.h"
#include "GA_UseItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UGA_UseItem : public UPRGameplayAbilityBase
{
	GENERATED_BODY()


private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UAnimMontage* initMontage;
	UAnimMontage* finishMontage;

	UPROPERTY(EditDefaultsOnly, Category = "UseTactical")
	FGameplayTag startAimingTag;

	UPROPERTY(EditDefaultsOnly, Category = "UseTactical")
	FGameplayTag throwTag;

	UPROPERTY(EditDefaultsOnly, Category = "UseTactical")
	FName socketSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "UseTactical")
	TSubclassOf<class AGameplayAbilityTargetActor> targetActorClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	FGameplayTag TacticalDamageEventTag;


	UFUNCTION()
	void CastStartMontageEnded();

	UFUNCTION()
	void ApplyEffectOrDamage(FGameplayEventData Payload);

	UFUNCTION()
	void StartTargetting(FGameplayEventData Payload);

	UFUNCTION()
	void ThrowTactical(FGameplayEventData Payload);

	float powerToThrow;
	FVector targetActorLocation;

	UFUNCTION()
	void TargetInRadius(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void TargettingCanceled(const FGameplayAbilityTargetDataHandle& Data);


	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
};
