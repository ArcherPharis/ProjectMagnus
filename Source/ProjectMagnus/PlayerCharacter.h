// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "PlayerCharacter.generated.h"

class ABaseEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitGiven, APlayerCharacter*, unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnUnitTarget, ACharacter_Base*, target, float, currentHealth, float, maxHealth, float, currentArmor, float, maxArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnEnemyUnitTarget, ABaseEnemy*, target, float, currentHealth, float, maxHealth, float, currentArmor, float, maxArmor, int, toKill, int, toBreak);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateHealthStamRange, float, maxHealth, float, maxStam,float, exp, float, maxExp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedDeploy, APlayerCharacter*, charaToDeploy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisplayTargetInfo, bool, display);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnInitiateLevelUp, APlayerCharacter*, unitData, float, oldHealth, float, oldStam, float, oldStr, float, oldEnd, float, oldAgi, float, oldDex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowUnitMenu);

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API APlayerCharacter : public ACharacter_Base
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	FOnUnitGiven onUnitGiven;
	FOnUpdateHealthStamRange onUpdateHealthStamRange;
	FOnUnitTarget onUnitTarget;
	FOnEnemyUnitTarget onEnemyUnitTarget;
	FOnClickedDeploy onUnitDeployed;
	FOnDisplayTargetInfo onDisplayTargetInfo;
	FOnInitiateLevelUp onInitiateLevelUp;
	FOnShowUnitMenu onShowUnitMenu;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Camera")
	class USpringArmComponent* GetSpringArm() { return springArm; }
	UFUNCTION(BlueprintPure, Category = "Camera")
	class UCameraComponent* GetCameraEye() { return playerEye; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Camera")
	void DeathEvent();


	void OnDeployed();
	virtual void LevelUp() override;

	

	virtual void StopAiming() override;
	virtual void OnUnitDeath(class ACharacter_Base* characterToDie) override;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* playerEye;

	void ReenableAILogic();
	void DisplayTargetInfo();

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* springArm;

	void GenerateHealthLevelUp(float& oldHealth);
	void GenerateStaminaLevelUp(float& oldStam);
	void GenerateStrengthLevelUp(float& oldStrength);
	void GenerateEnduranceLevelUp(float& oldEnd);
	void GenerateAgilityLevelUp(float& oldAgi);
	void GenerateDexterityLevelUp(float& oldDex);


	UPROPERTY(EditAnywhere, Category = "Camera")
	float aimFOV;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float nonAimFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	float aimOffset = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int healthGrowthRate = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int healthMinRoll = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int healthMaxRoll = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int staminaGrowthRate = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int staminaMinRoll = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int staminaMaxRoll = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int strengthGrowthRate = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int strengthMinRoll = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int strengthMaxRoll = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int enduranceGrowthRate = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int enduranceMinRoll = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int enduranceMaxRoll = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int agilityGrowthRate = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int agilityMinRoll = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int agilityMaxRoll = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int dexterityGrowthRate = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int dexterityMinRoll = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Mechanics")
	int dexterityMaxRoll = 1;



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

	void OpenUnitMenu();
	
	virtual void Attack() override;
	virtual void StopAttack() override;
	bool bHasAlreadyStartedMoving;


};
