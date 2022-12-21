// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Reload.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character_Base.h"
#include "Weapon.h"

void UGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//|| !K2_CheckAbilityCooldown()
	if (!K2_CheckAbilityCost())
	{
		return;
	}


	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	UAbilityTask_PlayMontageAndWait* ReloadMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, reloadMontage);

	if (ReloadMontageTask)
	{
		//ReloadMontageTask->OnBlendOut.AddDynamic(this, &UGA_Reload::ReloadMontageFinished);
		//ReloadMontageTask->OnCancelled.AddDynamic(this, &UGA_Reload::ReloadMontageFinished);
		//ReloadMontageTask->OnInterrupted.AddDynamic(this, &UGA_Reload::ReloadMontageFinished);
		ReloadMontageTask->OnCompleted.AddDynamic(this, &UGA_Reload::ReloadMontageFinished);
		ReloadMontageTask->ReadyForActivation();
	}
}

void UGA_Reload::ReloadMontageFinished()
{
	K2_CommitAbility();
	GetAvatarAsCharacter()->GetCurrentWeapon()->ReloadWeapon();
	K2_EndAbility();
}

void UGA_Reload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
