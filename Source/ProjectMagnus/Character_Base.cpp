// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"
#include "Weapon.h"
#include "StatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	statComponenet = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	GiveEquipment();
	originalSpeedValue = GetCharacterMovement()->GetMaxSpeed();
	sprintValue = GetCharacterMovement()->GetMaxSpeed() * speedMultiplier;
	aimSpeedValue = originalSpeedValue / 2;
	
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

bool ACharacter_Base::IsCharacterSprinting()
{
	if (GetVelocity().Length() > 600)
	{
		return true;
	}


	return false;
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
	if(!bIsAiming)
	GetCharacterMovement()->MaxWalkSpeed = sprintValue;
}

void ACharacter_Base::StopSprint()
{
	if (!bIsAiming)
	GetCharacterMovement()->MaxWalkSpeed = originalSpeedValue;

}

