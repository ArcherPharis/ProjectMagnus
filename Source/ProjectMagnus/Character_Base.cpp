// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"
#include "Weapon.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	if (weaponClass)
	{
		AWeapon* weapon;
		weapon = GetWorld()->SpawnActor<AWeapon>(weaponClass);
		weapon->SetOwner(this);
		weapon->OnEquip(GetMesh());
		equippedWeapon = weapon;
	}
	
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
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACharacter_Base::Aim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACharacter_Base::StopAiming);
		

}

void ACharacter_Base::Aim()
{
	bIsAiming = true;
	bUseControllerRotationYaw = true;
}

void ACharacter_Base::StopAiming()
{
	bIsAiming = false;
	bUseControllerRotationYaw = false;
}

