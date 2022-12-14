// Fill out your copyright notice in the Description page of Project Settings.


#include "GASprint.h"
#include "Character_Base.h"
#include "Kismet/GameplayStatics.h"
#include "PRAbilitySystemComponent.h"
#include "GameplayTagContainer.h"

void UGASprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	character = Cast<ACharacter_Base>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	character->onStoppedSprinting.AddDynamic(this, &UGASprint::StopSprinting);
	character->SetSprinting(true);
	character->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(normalDrainEffectTag);
	FGameplayEffectContextHandle handle;
	character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(SprintDrainEffect.GetDefaultObject(), -1, handle);
	

}

void UGASprint::StopSprinting()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint is getting event"));
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	character->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(sprintDrainEffectTag);
	character->SetSprinting(false);
	FGameplayEffectContextHandle handle;
	character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(NormalDrainEffect.GetDefaultObject(), -1, handle);

}
