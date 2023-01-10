// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "PRAbilityTypes.h"
#include "BaseUnitClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTMAGNUS_API UBaseUnitClass : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseUnitClass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UPRAttributeSet* GetOwnersAttributeSet() const { return attributeSet; }
	class UPRGameplayAbilityBase* GetClassFieldAbility() const { return fieldAbilityObj; }
	class UPRGameplayAbilityBase* GetClassAbilityOne() const { return abilityOneObj; }

	
	void GiveClassBonuses(class ACharacter_Base* ownerCharacter);

	FORCEINLINE FName GetUnitClassName() const { return ClassName; }

private:

	void GiveFieldAbility(ACharacter_Base* owner);
	void GiveClassAbilityOne(ACharacter_Base* owner);

	FGameplayAbilitySpecHandle FieldAbilitySpecHandle;
	FGameplayAbilitySpecHandle AbilityOneSpecHandle;


	UPRAttributeSet* attributeSet;

	UPRGameplayAbilityBase* fieldAbilityObj;
	UPRGameplayAbilityBase* abilityOneObj;

	UPROPERTY(EditDefaultsOnly, Category = "Field Ability")
	TSubclassOf<class UGameplayAbility> fieldAbility;

	UPROPERTY(EditDefaultsOnly, Category = "ClassAbilities")
	TSubclassOf<class UGameplayAbility> classAbilityOne;

	UPROPERTY(EditDefaultsOnly, Category = "Class Specific Bonuses")
	TSubclassOf<class UGameplayEffect> classBonusEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Class Default Info")
	FName ClassName = "Class Name";


		
};
