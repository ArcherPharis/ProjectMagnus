// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUnitClass.h"
#include "Character_Base.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PRGameplayAbilityBase.h"
#include "BaseAbilityToolTip.h"
#include "PRAttributeSet.h"

// Sets default values for this component's properties
UBaseUnitClass::UBaseUnitClass()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseUnitClass::BeginPlay()
{
	Super::BeginPlay();
	ACharacter_Base* owner = Cast<ACharacter_Base>(GetOwner());
	if (owner)
	{
		attributeSet = owner->GetAttributeSet();
		GiveFieldAbility(owner);
		GiveClassAbilityOne(owner);


	}

	// ...
	
}


// Called every frame
void UBaseUnitClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// ...
}



void UBaseUnitClass::GiveClassBonuses(ACharacter_Base* ownerCharacter)
{
	FGameplayEffectContextHandle handle;
	ownerCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(classBonusEffect.GetDefaultObject(), -1, handle);
}


void UBaseUnitClass::GiveFieldAbility(ACharacter_Base* owner)
{
	FieldAbilitySpecHandle = owner->GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(fieldAbility, 1, 6));
	fieldAbilityObj = Cast<UPRGameplayAbilityBase>(owner->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(FieldAbilitySpecHandle)->Ability);
	owner->AddSkillToList(fieldAbilityObj);

}

void UBaseUnitClass::GiveClassAbilityOne(ACharacter_Base* owner)
{
	AbilityOneSpecHandle = owner->GetAbilitySystemComponent()->GiveAbility(classAbilityOne);
	abilityOneObj = Cast<UPRGameplayAbilityBase>(owner->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilityOneSpecHandle)->Ability);
	owner->AddSkillToList(abilityOneObj);
}


