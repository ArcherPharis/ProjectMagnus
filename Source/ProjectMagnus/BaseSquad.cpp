// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSquad.h"
#include "Character_Base.h"

// Sets default values
ABaseSquad::ABaseSquad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseSquad::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<ACharacter_Base>(squadMembers[1], GetActorLocation(), GetActorRotation(), params);
	
}

// Called every frame
void ABaseSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

