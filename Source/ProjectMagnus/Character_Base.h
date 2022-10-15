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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Aim();
	virtual void StopAiming();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class UStatComponent* GetStatComponent() const { return statComponenet; }

	float GetCurrentWeight();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<class AWeapon> weaponClass;


	UPROPERTY(VisibleAnywhere, Category = "Gear")
	AWeapon* equippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerUnit")
	class UStatComponent* statComponenet;

	bool bIsAiming = false;

	void Attack();

	void StopAttack();

	void GiveEquipment();

	



};
