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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityAdded, UPRGameplayAbilityBase*, newAbility);

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

	FOnWeaponEquipped onWeaponEquipped;
	FOnAbilityAdded onAbilityAdded;

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

	UFUNCTION(BlueprintCallable, Category = "GameplayAbilities")
	void ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply);

	float GetWalkSpeed() const { return originalSpeedValue; }
	float GetRunSpeed() const { return sprintValue; }

	bool IsCharacterSprinting();

	void UseItem();
	



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

	
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	FORCEINLINE class UPRAttributeSet* GetAttributeSet() const { return attributeSet; }
	

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

	UPROPERTY()
	class UPRAbilitySystemComponent* abilitySystemComp;

	UPROPERTY()
	UPRAttributeSet* attributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TArray<TSubclassOf<class UGameplayEffect>> InitialEffects;


	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TMap<EPRAbilityInputID, TSubclassOf<UGameplayAbility>> InitialAbilities;

	void GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID = -1, bool broadCast = false);


	

	



};
