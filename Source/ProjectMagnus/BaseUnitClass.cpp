// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUnitClass.h"
#include "Character_Base.h"
#include "PRAttributeSet.h"

// Sets default values for this component's properties
UBaseUnitClass::UBaseUnitClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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


