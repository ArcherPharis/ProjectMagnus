// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "PRAbilityTypes.h"
#include "UnitAIInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Character_Base.generated.h"

class UPRGameplayAbilityBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquipped, AWeapon*, weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAPGauge, float, APAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityAdded, UPRGameplayAbilityBase*, newAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoppedSprinting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisplayTip, FString, message);



UCLASS()
class PROJECTMAGNUS_API ACharacter_Base : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface, public IUnitAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Base();
	void Interact();
	void ToggleUseControlRotationYaw(bool usesYaw);

	class APMGameModeBase* GetGameMode() const { return gameMode; }
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
	FOnDisplayTip onDisplayTip;


	void Attack();
	void EnemyTurnAttack();

	void Sprint();
	void StopSprint();
	void GiveEquipment();
	virtual void StopAiming();
	TSubclassOf<APawn> GetDeathPawnClass() const { return deathPawnClass; }

	UFUNCTION(BlueprintImplementableEvent, Category = "After Firing Event")
	void PostFireEventEffects();

	UFUNCTION(BlueprintImplementableEvent, Category = "After Firing Event")
	void StopAimMovement();

	UFUNCTION(BlueprintImplementableEvent, Category = "After Firing Event")
	void BeginAimMovement();

	void SetLogicEnabled(bool bIsLogicEnabled);


	void ToggleInput(bool enableInput);

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }

	void ToggleMessageOffWTimer(float time);
	void TurnOffMessage();

	bool GetIsUsingGear() const { return isUsingAbilityGear; }
	void SetIsUsingGear(bool newValue);

	UPRGameplayAbilityBase* GetUniqueSkillOne() const { return uniqueSkillOneObj; }
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Aim();
	virtual void PlayerAttack();
	virtual void StopAttack();
	void StopFiring();

	

	virtual void CharacterDied(const FOnAttributeChangeData& AttributeData);
	virtual void StaminaChange(const FOnAttributeChangeData& AttributeData);

	virtual void LevelUp();

	UFUNCTION(BlueprintCallable, Category = "GameplayAbilities")
	void ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply);

	float GetWalkSpeed() const { return originalSpeedValue; }
	float GetRunSpeed() const { return sprintValue; }

	bool IsCharacterOutOfStamina();

	void UseItem();

	void GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID = -1, bool broadCast = false);
	
	void UseSkillsOnDeploy();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "AttributeSet")
	void GetArmorValue(float& armor, float& maxArmor);

	void AfterUnitDeath();

	class ATacticalGear* GetTacticalGear() const { return CurrentlyEquippedTacticalGear; }
	class ASupportGear* GetSupportGear() const { return CurrentlyEquippedSupportGear; }

	AActor* GetKiller() const { return Killer; }
	void SetKiller(AActor* killer);

	void HandleCharacterDeath();

	void PlayGunAttackClip();

	class AAIController* GetAIController() const { return aiController; }
	void AIControllerRepossess();


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ReturnToTacActor();

	virtual void OnUnitDeath(class ACharacter_Base* characterToDie);
	
	float GetCurrentWeight();

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	class AWeapon* GetCurrentWeapon() { return equippedWeapon; }

	UFUNCTION()
	void GunAttackEventEnd();

	class UTexture2D* GetUnitPortrait() const { return unitPortrait; }
	class FName GetUnitName() const { return unitName; }

	void ChangeAP(int amount);

	bool CharacterCanAct();

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")
	void OnStopMoving();


	void SetSprinting(bool value);
	bool GetIsDead() { return isDead; }

	virtual class UBaseUnitClass* GetBaseUnitClass();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	FORCEINLINE class UPRAttributeSet* GetAttributeSet() const { return attributeSet; }

	void AddSkillToList(class UPRGameplayAbilityBase* ability);
	

private:

	UPROPERTY(EditDefaultsOnly, Category = "Interacter")
	class UInteracter* interacter;

	UPROPERTY(VisibleAnywhere, Category = "CurrentAbilities")
	TArray<UPRGameplayAbilityBase*> currentSkills;

	UPROPERTY(EditAnywhere, Category = "Team")
	FGenericTeamId TeamID;

	AAIController* aiController;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<AWeapon> weaponClass;
	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<class ATacticalGear> tacticalGearClass;
	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<class ASupportGear> supportGearClass;
	UPROPERTY(VisibleAnywhere, Category = "Gear")
	ATacticalGear* CurrentlyEquippedTacticalGear;
	UPROPERTY(VisibleAnywhere, Category = "Gear")
	ASupportGear* CurrentlyEquippedSupportGear;


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

	UPROPERTY(EditDefaultsOnly, Category = "Unique Skills")
	TSubclassOf<UGameplayAbility> uniqueSkillOne;
	UPRGameplayAbilityBase* uniqueSkillOneObj;


	bool isSprinting = false;
	bool isDead = false;
	bool isMoving;

	AActor* Killer;

	FTimerHandle returnToUnitHandle;
	
	APMGameModeBase* gameMode;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TMap<EPRAbilityInputID, TSubclassOf<UGameplayAbility>> InitialAbilities;


	
	void DrainStamina();
	void GiveUniqueClassSkills();
	
	
	bool isUsingAbilityGear = false;

	



};
