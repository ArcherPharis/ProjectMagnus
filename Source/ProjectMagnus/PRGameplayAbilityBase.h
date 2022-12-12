// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PRGameplayAbilityBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityCommited);

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UPRGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(BlueprintAssignable, Category = "GameplayAbilityBase")
	FOnAbilityCommited onAbilityActivated;

protected:
	FORCEINLINE class ACharacter_Base* GetAvatarAsCharacter() const { return AvatarCharacterBase; }

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;


private:
	ACharacter_Base* AvatarCharacterBase;
};
