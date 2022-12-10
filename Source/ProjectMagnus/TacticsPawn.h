// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TacticsPawn.generated.h"

UCLASS()
class PROJECTMAGNUS_API ATacticsPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATacticsPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "TacticsPawn")
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, Category = "TacticsPawn")
	class UCameraComponent* tacticsEye;

};
