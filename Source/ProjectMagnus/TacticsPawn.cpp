// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticsPawn.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATacticsPawn::ATacticsPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	tacticsEye = CreateDefaultSubobject<UCameraComponent>(TEXT("TacticalEye"));
	tacticsEye->SetupAttachment(RootComp);

}

// Called when the game starts or when spawned
void ATacticsPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATacticsPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATacticsPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

