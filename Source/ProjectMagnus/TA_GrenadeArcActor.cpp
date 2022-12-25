// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_GrenadeArcActor.h"
#include "Abilities/GameplayAbility.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ATA_GrenadeArcActor::ATA_GrenadeArcActor()
{
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);
	blastRadius = CreateDefaultSubobject<USphereComponent>(TEXT("TargetArea"));
	blastRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
	blastRadius->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATA_GrenadeArcActor::ConfirmTargetingAndContinue()
{
	FGameplayAbilityTargetData_LocationInfo* LocInfo = new FGameplayAbilityTargetData_LocationInfo;
	LocInfo->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	LocInfo->TargetLocation.LiteralTransform = GetActorTransform();


	FGameplayAbilityTargetDataHandle handle;
	handle.Add(LocInfo);
	TargetDataReadyDelegate.Broadcast(handle);

}

void ATA_GrenadeArcActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector ViewLoc;
	FRotator ViewRot;
	GetOwner()->GetActorEyesViewPoint(ViewLoc, ViewRot);
	FHitResult traceResult;
	if (GetWorld()->LineTraceSingleByChannel(traceResult, ViewLoc, ViewLoc + ViewRot.Vector() * throwDistance, ECC_Visibility))
	{
		SetActorLocation(traceResult.ImpactPoint);
	}
}
