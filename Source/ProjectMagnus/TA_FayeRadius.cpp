// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_FayeRadius.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"

ATA_FayeRadius::ATA_FayeRadius()
{
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);
	radius = CreateDefaultSubobject<USphereComponent>(TEXT("TargetArea"));
	radius->SetupAttachment(rootComp);
	radius->SetCollisionResponseToAllChannels(ECR_Ignore);
	radius->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATA_FayeRadius::BeginPlay()
{
	Super::BeginPlay();
}

void ATA_FayeRadius::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATA_FayeRadius::ConfirmTargeting()
{
	TArray<AActor*> OverlappingActors;
	radius->GetOverlappingActors(OverlappingActors);
	//OverlappingActors.Remove(OwningAbility->GetAvatarActorFromActorInfo());
	FGameplayAbilityTargetData_ActorArray* TargetActors = new FGameplayAbilityTargetData_ActorArray;
	for (auto target : OverlappingActors)
	{
		TargetActors->TargetActorArray.Add(target);
	}



	FGameplayAbilityTargetDataHandle handle;
	handle.Add(TargetActors);
	TargetDataReadyDelegate.Broadcast(handle);
	
}

void ATA_FayeRadius::ConfirmTargetingAndContinue()
{
	TArray<AActor*> OverlappingActors;
	radius->GetOverlappingActors(OverlappingActors);
	OverlappingActors.Remove(OwningAbility->GetAvatarActorFromActorInfo());
	FGameplayAbilityTargetData_ActorArray* TargetActors = new FGameplayAbilityTargetData_ActorArray;
	for (auto target : OverlappingActors)
	{
		TargetActors->TargetActorArray.Add(target);
	}



	FGameplayAbilityTargetDataHandle handle;
	handle.Add(TargetActors);
	TargetDataReadyDelegate.Broadcast(handle);
}
