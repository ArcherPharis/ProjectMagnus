// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "Character_Base.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	currentStamina = Stamina;
	ownerCharacter = Cast<ACharacter_Base>(GetOwner());
	staminaReductionBonus = (Strength + Speed) / 10;

	// ...
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::DrainStamina(float drainAmount)
{
	if (ownerCharacter)
	{
		currentEquipmentWeight = ownerCharacter->GetCurrentWeight();
		float trueDrain = (drainAmount / staminaReductionBonus) * currentEquipmentWeight;
		currentStamina -= trueDrain;
		currentStamina = FMath::Clamp(currentStamina, 0, Stamina);
	}
}

