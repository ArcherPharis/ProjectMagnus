// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Faye.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UGA_Faye : public UGameplayAbility
{
	GENERATED_BODY()


private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(EditDefaultsOnly, Category = "Faye")
	UAnimMontage* fayeMontage;


	UPROPERTY(EditDefaultsOnly, Category = "Faye")
	TSubclassOf<class AGameplayAbilityTargetActor> targetActorClass;
	UPROPERTY(EditDefaultsOnly, Category = "Faye")
	TSubclassOf<UGameplayEffect> effect;

	TArray<class AActor*> targets;

	UFUNCTION()
	void FayeMontageFinished();

	UFUNCTION()
	void InterruptedAbility();



	UFUNCTION()
	void FinishAbility(const FGameplayAbilityTargetDataHandle& Data);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
};
