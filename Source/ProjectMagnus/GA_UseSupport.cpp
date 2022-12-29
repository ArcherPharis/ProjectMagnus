// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_UseSupport.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character_Base.h"
#include "PRAbilitySystemComponent.h"
#include "PRAttributeSet.h"
#include "GameplayTagContainer.h"
#include "SupportGear.h"

void UGA_UseSupport::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if (!K2_CheckAbilityCost())
	{
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GetAvatarAsCharacter()->GetSupportGear()->GetUses() == 0)
	{
		GetAvatarAsCharacter()->GetAttributeSet()->SetActionPoints(GetAvatarAsCharacter()->GetAttributeSet()->GetActionPoints() + 1);
		K2_EndAbility();
		return;
	}


	useGearMontage = GetAvatarAsCharacter()->GetSupportGear()->GetBeginUseMontage();
	GetAvatarAsCharacter()->GetSupportGear()->AttatchToCharacterHand(GetAvatarAsCharacter()->GetMesh());
	UAbilityTask_PlayMontageAndWait* UseItemMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, useGearMontage);

	if (UseItemMontage)
	{
		//ReloadMontageTask->OnBlendOut.AddDynamic(this, &UGA_Reload::ReloadMontageFinished);
		//ReloadMontageTask->OnCancelled.AddDynamic(this, &UGA_Reload::ReloadMontageFinished);
		//ReloadMontageTask->OnInterrupted.AddDynamic(this, &UGA_Reload::ReloadMontageFinished);
		UseItemMontage->OnCompleted.AddDynamic(this, &UGA_UseSupport::UseStimMontageFinished);
		UseItemMontage->ReadyForActivation();
	}


}

void UGA_UseSupport::UseStimMontageFinished()
{
	FGameplayEffectContextHandle handle;
	GetAvatarAsCharacter()->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(GetAvatarAsCharacter()->GetSupportGear()->GetGameplayEffect().GetDefaultObject(), -1, handle);
	GetAvatarAsCharacter()->GetSupportGear()->UseGear();
	K2_EndAbility();
}

void UGA_UseSupport::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	GetAvatarAsCharacter()->GetSupportGear()->AttachToCharacterMesh(GetAvatarAsCharacter()->GetMesh());
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
