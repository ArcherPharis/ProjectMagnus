// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponUse, int, ammoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnForecastInfo, int, shotsToKill, int, shotsLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClearForecast);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginAttackEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledTargetWithGun, class ACharacter_Base*, charaThatDied);

UCLASS()
class PROJECTMAGNUS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	FOnWeaponUse onWeaponUse;
	FOnForecastInfo onForecastInfo;
	FOnClearForecast onClearForecast;
	FOnBeginAttackEvent onBeginAttackEvent;
	FOnKilledTargetWithGun onKilledTargetWithGun;

	void SetPlayerWantsToStopFiring(bool value);
	bool GetPlayerWantsToStopFiring() { return playersWantsToStopFiring; }
	void SetInAttackEvent(bool value);



	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool GetInAttackEvent() { return inAttackEvent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	


	void PlayWeaponSound(USceneComponent* firePoint);

	class ACharacter_Base* GetWeaponOwner() const { return myOwner; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnEquip(USkeletalMeshComponent* ownerMesh);

	float GetWeight() const { return weight; }

	virtual void Attack();

	UAnimMontage* GetAttackMontage() const { return attackMontage; }

	bool bFireButtonPressed = false;

	USkeletalMeshComponent* GetWeaponMesh() const { return mesh; }

	UTexture2D* GetWeaponCrosshair() const { return crossHair; }

	UTexture2D* GetWeaponIcon() const { return weaponIcon; }

	int GetCurrentAmmo() const { return currentAmmo; }
	void ChangeCurrentAmmo(int amt);

	void SetWeaponOwner(ACharacter_Base* owner);

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USceneComponent* root;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USkeletalMeshComponent* mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* attackAudio;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName socketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* attackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UTexture2D* crossHair;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UTexture2D* weaponIcon;

	ACharacter_Base* myOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int currentAmmo = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int maxAmmo = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int ammoReserves = 30;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float weight = 0.0f;

	bool playersWantsToStopFiring = false;

	bool inAttackEvent = false;



};
