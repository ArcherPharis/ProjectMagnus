// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"
#include "Weapon.h"
#include "StatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PRAbilitySystemComponent.h"
#include "PRGameplayAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PRAttributeSet.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	abilitySystemComp = CreateDefaultSubobject<UPRAbilitySystemComponent>(TEXT("Ability System Comp"));
	attributeSet = CreateDefaultSubobject<UPRAttributeSet>(TEXT("Attribute Set"));

}



void ACharacter_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	abilitySystemComp->InitAbilityActorInfo(this, this);
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	ApplyInitialEffect();
	GiveAbility(SprintAbility);
	for (auto& abilityKeyValuePair : InitialAbilities)
	{
		GiveAbility(abilityKeyValuePair.Value, static_cast<int>(abilityKeyValuePair.Key), true);
	}
	
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrainStamina();

}

// Called to bind functionality to input
void ACharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

float ACharacter_Base::GetCurrentWeight()
{
	return equippedWeapon->GetWeight();
}

void ACharacter_Base::ChangeAP(int amount)
{
	GetAttributeSet()->SetActionPoints(GetAttributeSet()->GetActionPoints() + amount);
}

bool ACharacter_Base::CharacterCanAct()
{
	if (GetAttributeSet()->GetActionPoints() > 0)
	{
		return true;
	}
	return false;
}

void ACharacter_Base::SetSprinting(bool value)
{
	isSprinting = value;
}

UAbilitySystemComponent* ACharacter_Base::GetAbilitySystemComponent() const
{
	return abilitySystemComp;
}

void ACharacter_Base::GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID, bool broadCast)
{
	FGameplayAbilitySpecHandle AbilitySpecHandle = abilitySystemComp->GiveAbility(FGameplayAbilitySpec(newAbility, 1, inputID));

	if (broadCast)
	{
		onAbilityAdded.Broadcast(Cast<UPRGameplayAbilityBase>(GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability));
	}
}

void ACharacter_Base::DrainStamina()
{
	if (isSprinting) return;
	if (GetVelocity().Length() > 0 && !isMoving)
	{
		isMoving = true;
		FGameplayEffectContextHandle handle;
		GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(staminaDrainEffect.GetDefaultObject(), -1, handle);
	}
	else if (GetVelocity().Length() == 0)
	{

		if (isMoving == false) return;
		OnStopMoving();
		isMoving = false;
	}
}

void ACharacter_Base::Attack()
{
	equippedWeapon->bFireButtonPressed = true;
	if (bIsAiming)
	{
		equippedWeapon->Attack();
	}
}

void ACharacter_Base::StopAttack()
{
	equippedWeapon->bFireButtonPressed = false;

}


void ACharacter_Base::ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply)
{
	FGameplayEffectSpecHandle InitialSpec = abilitySystemComp->MakeOutgoingSpec(effectToApply, -1, abilitySystemComp->MakeEffectContext());
	abilitySystemComp->ApplyGameplayEffectSpecToSelf(*InitialSpec.Data);
}

void ACharacter_Base::ApplyInitialEffect()
{
	for (auto effect : InitialEffects)
	{
		ApplyEffectToSelf(effect);
	}
}

bool ACharacter_Base::IsCharacterOutOfStamina()
{
	if (GetAttributeSet()->GetStamina() == 0)
	{
		return true;
	}

	return false;
}

void ACharacter_Base::UseItem()
{
	
}

void ACharacter_Base::GiveEquipment()
{
	if (weaponClass)
	{
		AWeapon* weapon;
		weapon = GetWorld()->SpawnActor<AWeapon>(weaponClass);
		weapon->SetOwner(this);
		weapon->OnEquip(GetMesh());
		equippedWeapon = weapon;
		onWeaponEquipped.Broadcast(equippedWeapon);

	}

}


void ACharacter_Base::Aim()
{

	bIsAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = aimSpeedValue;

	
}

void ACharacter_Base::StopAiming()
{
	bIsAiming = false;
	GetCharacterMovement()->MaxWalkSpeed = originalSpeedValue;
	StopAttack();

	
}

void ACharacter_Base::Sprint()
{
	if (!bIsAiming)
	abilitySystemComp->TryActivateAbilityByClass(SprintAbility);
}

void ACharacter_Base::StopSprint()
{
	if (!bIsAiming)
	onStoppedSprinting.Broadcast();

}

