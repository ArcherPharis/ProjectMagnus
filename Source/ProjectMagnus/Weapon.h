// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponUse, int, ammoCount);

UCLASS()
class PROJECTMAGNUS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	FOnWeaponUse onWeaponUse;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayWeaponSound(USceneComponent* firePoint);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnEquip(USkeletalMeshComponent* ownerMesh);

	float GetWeight() const { return weight; }

	virtual void Attack();

	bool bFireButtonPressed = false;

	USkeletalMeshComponent* GetWeaponMesh() const { return mesh; }

	UTexture2D* GetWeaponCrosshair() const { return crossHair; }

	UTexture2D* GetWeaponIcon() const { return weaponIcon; }

	int GetCurrentAmmo() const { return currentAmmo; }
	void ChangeCurrentAmmo(int amt);

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
	UTexture2D* crossHair;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UTexture2D* weaponIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int currentAmmo = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int maxAmmo = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int ammoReserves = 30;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float weight = 0.0f;



};
