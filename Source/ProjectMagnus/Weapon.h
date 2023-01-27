// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponUse, int, ammoCount, int, ammoReserves);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginAttackEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndAttackEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledTargetWithGun, class ACharacter_Base*, charaThatDied);


UENUM()
enum WeaponType
{
	Rifle UMETA(DisplayName = "Rifle"),
	Pistol UMETA(DisplayName = "Pistol"),
	MachineGun UMETA(DisplayName = "MachineGun"),
	Sniper UMETA(DisplayName = "Sniper"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Explosive UMETA(DisplayName = "Explosive"),
	OtherMelee UMETA(DisplayName = "Other/Melee")
};

UCLASS()
class PROJECTMAGNUS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	FOnWeaponUse onWeaponUse;
	FOnBeginAttackEvent onBeginAttackEvent;
	FOnEndAttackEvent onEndAttackEvent;
	FOnKilledTargetWithGun onKilledTargetWithGun;

	void SetPlayerWantsToStopFiring(bool value);
	bool GetPlayerWantsToStopFiring() { return playersWantsToStopFiring; }
	void SetInAttackEvent(bool value);

	bool IsWeaponEmpty();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool GetInAttackEvent() { return inAttackEvent; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetCanFire(bool bCanFire);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Reload();

	bool canfire = true;

	virtual bool CanAttack() const;
	
	float GetFireRate() const { return fireRate; }
	float GetDamage() const { return damage; }

	void PlayWeaponSound(USceneComponent* firePoint);

	class ACharacter_Base* GetWeaponOwner() const { return myOwner; }

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USceneComponent* firePoint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	void GetDamageInfo(class ABaseEnemy* enemy, int& toBreak, int& toKill);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnEquip(USkeletalMeshComponent* ownerMesh);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnUnequip(USkeletalMeshComponent* ownerMesh);

	float GetWeight() const { return weight; }

	virtual void Attack();

	virtual void AttackAI();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void AttackPointAnimNotify();

	UAnimMontage* GetAttackMontage() const { return attackMontage; }

	bool bFireButtonPressed = false;

	USkeletalMeshComponent* GetWeaponMesh() const { return mesh; }

	UTexture2D* GetWeaponCrosshair() const { return crossHair; }

	UTexture2D* GetWeaponIcon() const { return weaponIcon; }

	FORCEINLINE int GetCurrentAmmo() const { return currentAmmo; }
	FORCEINLINE int GetMaxAmmo() const { return maxAmmo; }
	FORCEINLINE int GetAmmoReserves() const { return ammoReserves; }
	FORCEINLINE FName GetWeaponName() const { return weaponName; }
	void ChangeCurrentAmmo(int amt);
	void ChangeAmmoReserves(int amt);
	void SetCurrentAmmo(int amt);
	void SetAmmoReserves(int amt);
	void AddToAmmoReserves(int amt);

	void SetWeaponOwner(ACharacter_Base* owner);

	virtual void ReloadWeapon();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	USceneComponent* GetFirePoint() const { return firePoint; }



	


private:

	UPROPERTY(EditDefaultsOnly, Category = "WeaponType")
	TEnumAsByte<WeaponType> weaponType;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* asAIUnitAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USceneComponent* root;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USkeletalMeshComponent* mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* attackAudio;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName equipSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName unequipSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* attackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UTexture2D* crossHair;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UTexture2D* weaponIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName weaponName;

	ACharacter_Base* myOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int currentAmmo = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int maxAmmo = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int ammoReserves = 30;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float weight = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float fireRate = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float damage;

	FTimerHandle FiringTimer;

	bool playersWantsToStopFiring = false;

	bool inAttackEvent = false;



};
