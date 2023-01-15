// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameplayEffectTypes.h"
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


	void FirearmAim();
	void StopFirearmAim();
	virtual void ReloadWeapon() override;
	

	
	

	

private:



	void DecrementAmmo();

	bool killedSomething = false;
	class ABaseEnemy* engagedEnemy;

	virtual void Attack() override;

	FVector WeaponSpread(FVector Endpoint);

	

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class UGameplayEffect> damageEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FGameplayTagContainer reloadTag;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* hitMarkerSound;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* impactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* muzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* hitEffect;


	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float weaponRange;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float minSpread = -10;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float maxSpread = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Fire Mode")
	TEnumAsByte<Firetype> fireMode;
	

	FTimerHandle fireDelayTimer;
	FTimerHandle aimCastTimerHandle;

	TArray<ACharacter_Base*>killedEnemies;

	AActor* potentialActor;
	AActor* targetedActor;

	FHitResult PotentialActorResult(FHitResult potResult);

	void AfterFireCheck();

	


};
