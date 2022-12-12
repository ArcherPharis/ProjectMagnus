// Fill out your copyright notice in the Description page of Project Settings.


#include "PRGameplayAbilityBase.h"
#include "PRAttributeSet.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character_Base.h"

void UPRGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AvatarCharacterBase = Cast<ACharacter_Base>(GetAvatarActorFromActorInfo());
}

bool UPRGameplayAbilityBase::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	if (Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		UPRGameplayAbilityBase* CDO = Cast<UPRGameplayAbilityBase>(GetClass()->GetDefaultObject());
		if (CDO)
		{
			CDO->onAbilityActivated.Broadcast();
		}
		return true;
	}
	return false;
}
