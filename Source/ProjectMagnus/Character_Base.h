// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquipped, AWeapon*, weapon);



UCLASS()
class PROJECTMAGNUS_API ACharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Base();

	UFUNCTION(BlueprintCallable, Category = "PlayerAiming")
	bool GetIsAiming() const { return bIsAiming; }

	FOnWeaponEquipped onWeaponEquipped;

	void Sprint();
	void StopSprint();
	void GiveEquipment();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Aim();
	virtual void StopAiming();
	virtual void Attack();
	virtual void StopAttack();

	float GetWalkSpeed() const { return originalSpeedValue; }
	float GetRunSpeed() const { return sprintValue; }

	bool IsCharacterSprinting();
	



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class UStatComponent* GetStatComponent() const { return statComponenet; }

	float GetCurrentWeight();

	class AWeapon* GetCurrentWeapon() { return equippedWeapon; }

	class UTexture2D* GetUnitPortrait() const { return unitPortrait; }
	class FName GetUnitName() const { return unitName; }
	

	

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<AWeapon> weaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Personal Info")
	UTexture2D* unitPortrait;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Personal Info")
	FName unitName;


	UPROPERTY(VisibleAnywhere, Category = "Gear")
	AWeapon* equippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerUnit")
	class UStatComponent* statComponenet;

	UPROPERTY(EditDefaultsOnly, Category = "Meta Stats")
	float originalSpeedValue = 600.f;
	float sprintValue  = 780.f;
	float aimSpeedValue = 300.f;

	bool bIsAiming = false;



	

	



};
