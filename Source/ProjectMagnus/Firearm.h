// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Firearm.generated.h"

UENUM()
enum Firetype
{
	Semiauto UMETA(DisplayName = "Semiautomatic"),
	Automatic UMETA(DisplayName = "Automatic")
};


/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API AFirearm : public AWeapon
{
	GENERATED_BODY()

public:
	AFirearm();


	UFUNCTION(BlueprintImplementableEvent, Category = "Gun Events")
	void OnAttack();
	UFUNCTION(BlueprintImplementableEvent, Category = "Gun Events")
	void OnStopAttack();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gun Events")
	void SpawnImpactEffects(const FHitResult result);

	

private:

	virtual void Attack() override;

	FVector WeaponSpread(FVector Endpoint);
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		USceneComponent* firePoint;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* muzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* hitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float damage;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float weaponRange;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float minSpread = -10;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float maxSpread = 10;



	UPROPERTY(EditDefaultsOnly, Category = "Fire Mode")
	TEnumAsByte<Firetype> fireMode;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float fireRate = 2.0f;

	bool canfire = true;

	FTimerHandle fireDelayTimer;

	void WeaponFire();
	void AfterFireCheck();


};
