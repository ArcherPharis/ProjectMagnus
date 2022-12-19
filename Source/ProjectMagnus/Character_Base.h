// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "PRAbilityTypes.h"
#include "Character_Base.generated.h"

class UPRGameplayAbilityBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquipped, AWeapon*, weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAPGauge, float, APAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityAdded, UPRGameplayAbilityBase*, newAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoppedSprinting);

UCLASS()
class PROJECTMAGNUS_API ACharacter_Base : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Base();

	void ApplyInitialEffect();
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "PlayerAiming")
	bool GetIsAiming() const { return bIsAiming; }

	UFUNCTION(BlueprintCallable, Category = "PlayerAiming")
	void SetIsAiming(bool value);

	FOnWeaponEquipped onWeaponEquipped;
	FOnAPGauge onAPGauge;
	FOnAbilityAdded onAbilityAdded;
	FOnStoppedSprinting onStoppedSprinting;

	void Sprint();
	void StopSprint();
	void GiveEquipment();
	virtual void StopAiming();
	TSubclassOf<APawn> GetDeathPawnClass() const { return deathPawnClass; }
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Aim();
	virtual void Attack();
	virtual void StopAttack();
	void StopFiring();

	UFUNCTION(BlueprintCallable, Category = "GameplayAbilities")
	void ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply);

	float GetWalkSpeed() const { return originalSpeedValue; }
	float GetRunSpeed() const { return sprintValue; }

	bool IsCharacterOutOfStamina();

	void UseItem();


	



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleCharacterDeath();

	void PlayGunAttackClip();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void OnUnitDeath(class ACharacter_Base* characterToDie);
	
	float GetCurrentWeight();

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	class AWeapon* GetCurrentWeapon() { return equippedWeapon; }

	class UTexture2D* GetUnitPortrait() const { return unitPortrait; }
	class FName GetUnitName() const { return unitName; }

	void ChangeAP(int amount);

	bool CharacterCanAct();

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")
	void OnStopMoving();


	void SetSprinting(bool value);
	bool GetIsDead() { return isDead; }

	
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	FORCEINLINE class UPRAttributeSet* GetAttributeSet() const { return attributeSet; }
	

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<AWeapon> weaponClass;


	UPROPERTY(EditDefaultsOnly, Category = "PossessablePawns")
	TSubclassOf<APawn> deathPawnClass;


	UPROPERTY(EditDefaultsOnly, Category = "Unit Personal Info")
	UTexture2D* unitPortrait;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Personal Info")
	USoundBase* GunAttackClip;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Personal Info")
	UAnimMontage* onDeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Personal Info")
	FName unitName;


	UPROPERTY(VisibleAnywhere, Category = "Gear")
	AWeapon* equippedWeapon;


	UPROPERTY(EditDefaultsOnly, Category = "Meta Stats")
	float originalSpeedValue = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Meta Stats")
	float sprintValue  = 780.f;
	UPROPERTY(EditDefaultsOnly, Category = "Meta Stats")
	float aimSpeedValue = 300.f;

	bool bIsAiming = false;

	UPROPERTY()
	class UPRAbilitySystemComponent* abilitySystemComp;

	UPROPERTY()
	UPRAttributeSet* attributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TArray<TSubclassOf<class UGameplayEffect>> InitialEffects;


	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TSubclassOf<UGameplayEffect> staminaDrainEffect;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TSubclassOf<UGameplayAbility> SprintAbility;

	bool isSprinting = false;
	bool isDead = false;
	bool isMoving;

	FTimerHandle returnToUnitHandle;
	void AfterUnitDeath();
	void SpawnDeathUnit();


	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TMap<EPRAbilityInputID, TSubclassOf<UGameplayAbility>> InitialAbilities;


	void GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID = -1, bool broadCast = false);
	void DrainStamina();
	void CharacterDied(const FOnAttributeChangeData& AttributeData);
	
	

	



};
