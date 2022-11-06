// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTMAGNUS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

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

	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float weight = 0.0f;



};
