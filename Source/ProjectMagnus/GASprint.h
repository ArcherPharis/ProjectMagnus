// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASprint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UGASprint : public UGameplayAbility
{
	GENERATED_BODY()

private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void StopSprinting();

	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	FGameplayTagContainer normalDrainEffectTag;

	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	FGameplayTagContainer sprintDrainEffectTag;


	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	TSubclassOf<class UGameplayEffect> SprintDrainEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	TSubclassOf<class UGameplayEffect> NormalDrainEffect;

	class ACharacter_Base* character;
};
