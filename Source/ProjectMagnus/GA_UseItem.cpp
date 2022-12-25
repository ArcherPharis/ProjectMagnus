// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_UseItem.h"
#include "Character_Base.h"
#include "Weapon.h"
#include "TacticalGear.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"

void UGA_UseItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	GetAvatarAsCharacter()->onDisplayTip.Broadcast("Using Tactical Gear costs 1 AP.");
	ACharacter_Base* user = GetAvatarAsCharacter();
	if (user)
	{
		user->BeginAimMovement();
		initMontage = user->GetTacticalGear()->GetBeginUseMontage();
		finishMontage = user->GetTacticalGear()->GetConfirmUseMontage();
		user->GetTacticalGear()->AttatchToCharacterHand(user->GetMesh());
		user->GetCurrentWeapon()->SetInAttackEvent(true);

		UAbilityTask_PlayMontageAndWait* StartMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, initMontage);

		if (StartMontageTask)
		{

			StartMontageTask->ReadyForActivation();
		}

		UAbilityTask_WaitGameplayEvent* WaitStartTargettingTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, startAimingTag, nullptr, false, false);

		if (WaitStartTargettingTask)
		{
			WaitStartTargettingTask->EventReceived.AddDynamic(this, &UGA_UseItem::StartTargetting);
			WaitStartTargettingTask->ReadyForActivation();
		}

		UAbilityTask_WaitGameplayEvent* WaitExplosion = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TacticalDamageEventTag, nullptr, false, false);
		if (WaitExplosion)
		{
			WaitExplosion->EventReceived.AddDynamic(this, &UGA_UseItem::ApplyEffectOrDamage);
			WaitExplosion->ReadyForActivation();
		}
	}
	
}

void UGA_UseItem::CastStartMontageEnded()
{
	GetAvatarAsCharacter()->GetTacticalGear()->AttachToCharacterMesh(GetAvatarAsCharacter()->GetMesh());

}

void UGA_UseItem::ApplyEffectOrDamage(FGameplayEventData Payload)
{

	UE_LOG(LogTemp, Warning, TEXT("Damage is ready to be applied."));


	if (Payload.TargetData.Num() == 0) return;

	for (TSharedPtr<FGameplayAbilityTargetData>& data : Payload.TargetData.Data)
	{
		for (TWeakObjectPtr<AActor>& actorWeakPtr : data->GetActors())
		{
			AActor* HitTarget = actorWeakPtr.Get();

		}
	}


	FGameplayEffectSpecHandle handle = MakeOutgoingGameplayEffectSpec(GetAvatarAsCharacter()->GetTacticalGear()->GetGameplayEffect(), -1);
	K2_ApplyGameplayEffectSpecToTarget(handle, Payload.TargetData);

	K2_EndAbility();
}

void UGA_UseItem::StartTargetting(FGameplayEventData Payload)
{
	//GetAvatarAsCharacter()->SetIsAiming(true);
	UAbilityTask_WaitTargetData* WaitTargettingTask = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::UserConfirmed, targetActorClass);
	//this is just wait target data
	if (WaitTargettingTask)
	{
		WaitTargettingTask->ValidData.AddDynamic(this, &UGA_UseItem::TargetInRadius);
		WaitTargettingTask->Cancelled.AddDynamic(this, &UGA_UseItem::TargettingCanceled);
		WaitTargettingTask->ReadyForActivation();

		AGameplayAbilityTargetActor* TargetActorSpawned;
		WaitTargettingTask->BeginSpawningActor(this, targetActorClass, TargetActorSpawned);

		TargetActorSpawned->SetOwner(GetAvatarActorFromActorInfo());
		WaitTargettingTask->FinishSpawningActor(this, TargetActorSpawned);

		
	}
	
}

void UGA_UseItem::ThrowTactical(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Getting to throw tactical"));
	GetAvatarAsCharacter()->GetTacticalGear()->ThrowGear(powerToThrow, targetActorLocation);
	GetAvatarAsCharacter()->GetCurrentWeapon()->SetInAttackEvent(false);
}

void UGA_UseItem::TargetInRadius(const FGameplayAbilityTargetDataHandle& Data)
{
	K2_CommitAbility();
	GetAvatarAsCharacter()->ToggleInput(false);
	FVector actorLoc = Data.Get(0)->GetEndPoint();
	targetActorLocation = actorLoc;
	float distance = FVector::Dist(actorLoc, GetAvatarAsCharacter()->GetActorLocation());
	powerToThrow = distance;

	//todo, we'd want to throw the gear after a throwing montage instead. we can just store
	//the distance here and then launch a montage event below. This montage would wait for
	//tag data from the montage instead of just sending data over.




	UAbilityTask_PlayMontageAndWait* StartMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, finishMontage);

	if (StartMontageTask)
	{
		StartMontageTask->ReadyForActivation();
	}



	UAbilityTask_WaitGameplayEvent* ThrowGrenadeTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, throwTag, nullptr, false, false);

	if (ThrowGrenadeTask)
	{
		ThrowGrenadeTask->EventReceived.AddDynamic(this, &UGA_UseItem::ThrowTactical);
		ThrowGrenadeTask->ReadyForActivation();
	}



	

}

void UGA_UseItem::TargettingCanceled(const FGameplayAbilityTargetDataHandle& Data)
{
	GetAvatarAsCharacter()->GetCurrentWeapon()->SetInAttackEvent(false);
	K2_EndAbility();
}

void UGA_UseItem::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//GetAvatarAsCharacter()->SetIsAiming(false);
	GetAvatarAsCharacter()->StopAimMovement();
	GetAvatarAsCharacter()->ToggleInput(true);
	GetAvatarAsCharacter()->GetCurrentWeapon()->SetInAttackEvent(false);
	GetAvatarAsCharacter()->onDisplayTip.Broadcast("");
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
