// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Reload.h"

void UGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//|| !K2_CheckAbilityCooldown()
	if (!K2_CheckAbilityCost())
	{
		return;
	}


	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	K2_CommitAbility();
}
