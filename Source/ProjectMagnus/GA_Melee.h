// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRGameplayAbilityBase.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Melee.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UGA_Melee : public UPRGameplayAbilityBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	float HitPushSpeed = 200.f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	FGameplayTag ComboChangeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	FGameplayTag ComboCommitTag;

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	FGameplayTag HitTag;

	UFUNCTION()
	void UpdateCombo(FGameplayEventData Payload);

	UFUNCTION()
	void MontageFinished();

	UFUNCTION()
	void ComboCommit(FGameplayEventData Payload);
	UFUNCTION()
	void Hit(FGameplayEventData Payload);

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	TSubclassOf<UGameplayEffect> HitEffect;

	FName NextComboSectionName;

	bool bHitPushed;
};
