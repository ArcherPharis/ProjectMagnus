// Fill out your copyright notice in the Description page of Project Settings.


#include "Gear.h"
#include "Character_Base.h"

// Sets default values
AGear::AGear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(rootComp);
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(rootComp);

}

// Called when the game starts or when spawned
void AGear::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGear::UseGear()
{
	SetUses(-1);
	if (Uses == 0)
	{
		SetActorHiddenInGame(true);
	}
	ACharacter_Base* owningChara = Cast<ACharacter_Base>(GetOwner());
	AttachToCharacterMesh(owningChara->GetMesh());
}

void AGear::AttachToCharacterMesh(USkeletalMeshComponent* characterMesh)
{
	AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, onCharacterMeshSocketName);
}

void AGear::AttatchToCharacterHand(USkeletalMeshComponent* characterMesh)
{
	AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, inCharacterHandSocketName);

}

void AGear::SetUses(int value)
{
	Uses += value;
}

