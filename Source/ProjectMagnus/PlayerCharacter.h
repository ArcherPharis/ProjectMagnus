// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitGiven, APlayerCharacter*, unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateHealthStamRange, float, maxHealth, float, maxStam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedDeploy, APlayerCharacter*, charaToDeploy);

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API APlayerCharacter : public ACharacter_Base
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	FOnUnitGiven onUnitGiven;
	FOnUpdateHealthStamRange onUpdateHealthStamRange;
	FOnClickedDeploy onUnitDeployed;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Camera")
	class USpringArmComponent* GetSpringArm() { return springArm; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Camera")
	void DeathEvent();


	void OnDeployed();

	virtual void StopAiming() override;
	virtual void OnUnitDeath(class ACharacter_Base* characterToDie) override;


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
	
	virtual void Attack() override;
	virtual void StopAttack() override;
	bool bHasAlreadyStartedMoving;


};
