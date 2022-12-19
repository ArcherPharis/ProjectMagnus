// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Character_Base.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	mesh->SetupAttachment(root);


}


void AWeapon::SetPlayerWantsToStopFiring(bool value)
{
	playersWantsToStopFiring = value;
}

void AWeapon::SetInAttackEvent(bool value)
{
	inAttackEvent = value;
}



// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	weight = weight / 10;
	
	
}


void AWeapon::PlayWeaponSound(USceneComponent* firePoint)
{
	UGameplayStatics::SpawnSoundAttached(attackAudio, firePoint);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnEquip(USkeletalMeshComponent* ownerMesh)
{
	AttachToComponent(ownerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
}

void AWeapon::Attack()
{
	
}

void AWeapon::ChangeCurrentAmmo(int amt)
{
	currentAmmo += amt;
}

void AWeapon::SetWeaponOwner(ACharacter_Base* owner)
{
	SetOwner(owner);
	myOwner = owner;
}


