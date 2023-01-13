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

	UFUNCTION(BlueprintCallable, Category = "GameplayAbilityBase")
	float GetCooldownDuration() const;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbilityBase")
	float GetCost() const;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbilityBase")
	FORCEINLINE UTexture2D* GetIcon() const { return AbilityIcon; }
	FORCEINLINE FText GetSkillName() const { return skillName; }
	FORCEINLINE FText GetSkillDesc() const { return skillDescription; }
	FORCEINLINE TSubclassOf<class UBaseAbilityToolTip> GetToolTipWidget() const { return toolTipWidget; }
	FORCEINLINE UBaseAbilityToolTip* GetToolTip() const { return toolTip; }
	void SetToolTip(UBaseAbilityToolTip* abilityTooltip);

	class ACharacter_Base* AvatarCharacterBase;

	bool IsASkillUsedOnDeploy() const{ return useSkillOnDeploy; }
	bool IsASkillUsedOnStaminaDepletion() const { return useSkillOnStaminaDepletion; }
	bool IsASkillUsedHealthThreshold() const { return useSkillOnHealthBelowHalf; }


protected:
	FORCEINLINE  ACharacter_Base* GetAvatarAsCharacter() const { return AvatarCharacterBase; }

	

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;


private:
	
	

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase")
	UTexture2D* AbilityIcon;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase")
	FText skillName;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase", meta = (MultiLine = true))
	FText skillDescription;


	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase")
	TSubclassOf<class UBaseAbilityToolTip> toolTipWidget;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase")
	bool useSkillOnDeploy = false;
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase")
	bool useSkillOnStaminaDepletion = false;
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase")
	bool useSkillOnHealthBelowHalf = false;

	UBaseAbilityToolTip* toolTip;
};
