// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Faye.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Kismet/GameplayStatics.h"
#include "Character_Base.h"

void UGA_Faye::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* FayeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, fayeMontage);


	if (FayeMontageTask)
	{
		FayeMontageTask->OnCancelled.AddDynamic(this, &UGA_Faye::InterruptedAbility);
		FayeMontageTask->OnInterrupted.AddDynamic(this, &UGA_Faye::InterruptedAbility);
		FayeMontageTask->OnCompleted.AddDynamic(this, &UGA_Faye::FayeMontageFinished);
		FayeMontageTask->ReadyForActivation();
	}


}

void UGA_Faye::FayeMontageFinished()
{
	UAbilityTask_WaitTargetData* WaitTargettingTask = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::Instant, targetActorClass);
	//this is just wait target data
	if (WaitTargettingTask)
	{
		WaitTargettingTask->ValidData.AddDynamic(this, &UGA_Faye::FinishAbility);
		WaitTargettingTask->ReadyForActivation();

		AGameplayAbilityTargetActor* TargetActorSpawned;
		WaitTargettingTask->BeginSpawningActor(this, targetActorClass, TargetActorSpawned);

		TargetActorSpawned->SetOwner(GetAvatarActorFromActorInfo());
		WaitTargettingTask->FinishSpawningActor(this, TargetActorSpawned);

		UE_LOG(LogTemp, Warning, TEXT("%s and my name is: %s"), *TargetActorSpawned->GetActorLocation().ToString(), *TargetActorSpawned->GetName());


	}


}

void UGA_Faye::InterruptedAbility()
{
	K2_EndAbility();
}


void UGA_Faye::FinishAbility(const FGameplayAbilityTargetDataHandle& Data)
{
	K2_CommitAbility();
	//BP_ApplyGameplayEffectToOwner(effect);
	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(effect), Data);
	K2_EndAbility();
}

void UGA_Faye::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
