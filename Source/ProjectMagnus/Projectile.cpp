// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character_Base.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);
	hitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	hitBox->SetupAttachment(mesh);
	moveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

}

void AProjectile::SetProjectileSpeed(float speed, FVector direction)
{
	moveComp->InitialSpeed = speed;
	GetPowerForImpulse(speed);
	FRotator lookAtTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), direction);
	SetActorRotation(lookAtTarget);
}

void AProjectile::SetEventTag(FGameplayTag tag)
{
	TacticalDamageEventTag = tag;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	hitBox->SetActive(false);
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &AProjectile::Explode, explodeTime, false);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation(), FRotator::ZeroRotator, FVector3d(6.0f, 6.0f,6.0f));
	hitBox->SetActive(true);
	TArray<AActor*> potentialTargets;
	hitBox->GetOverlappingActors(potentialTargets);
	FGameplayEventData eventData;
	FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(potentialTargets, true);
	eventData.TargetData = TargetData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner()->GetOwner(), TacticalDamageEventTag, eventData);
}


