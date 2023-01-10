// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHQ.h"
#include "Components/SphereComponent.h"

// Sets default values
APlayerHQ::APlayerHQ()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(RootComponent);
	hitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	hitSphere->SetupAttachment(RootComponent);
	


}

// Called when the game starts or when spawned
void APlayerHQ::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerHQ::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

