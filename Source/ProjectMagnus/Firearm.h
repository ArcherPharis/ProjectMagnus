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

	UFUNCTION(BlueprintImplementableEvent, Category = "Gun Events")
	void RotateCharacter(class ACharacter_Base* character, AActor* result);

	void FirearmAim();
	void StopFirearmAim();
	bool TryStopFiring();
	
	

	

private:

	virtual void Attack() override;

	FVector WeaponSpread(FVector Endpoint);
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USceneComponent* firePoint;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* hitMarkerSound;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* impactSound;

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
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Handleability = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float wielderControlPercent = 15.f;

	bool canfire = true;

	FTimerHandle fireDelayTimer;
	FTimerHandle aimCastTimerHandle;

	AActor* targetedActor;
	AActor* potentialActor;
	TArray<ACharacter_Base*>killedEnemies;

	FHitResult PotentialActorResult(FHitResult potResult);

	void WeaponFire();
	void BeginAttack();
	void AfterFireCheck();
	int GetBulletsToKill(AActor* currentTarget);

	FTimerHandle GunKilledHandle;
	void GunKilledTarget();
	


};
