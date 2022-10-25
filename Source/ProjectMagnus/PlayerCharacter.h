// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API APlayerCharacter : public ACharacter_Base
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Camera")
	class USpringArmComponent* GetSpringArm() { return springArm; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* playerEye;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* springArm;



	UPROPERTY(EditAnywhere, Category = "Camera")
	float aimFOV;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float nonAimFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	float aimOffset = 50.f;

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void LookRight(float value);

	UFUNCTION()
	void LookUp(float value);

	FRotator GetControlRotator();

	virtual void Aim() override;
	virtual void StopAiming() override;
	virtual void Attack() override;
	virtual void StopAttack() override;
	bool bHasAlreadyStartedMoving;


};
