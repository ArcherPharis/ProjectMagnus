// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, health, float, maxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChange, float, stam, float, maxStam);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTMAGNUS_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	FOnHealthChange onHealthChange;
	FOnStaminaChange onStamChange;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DrainStamina(float drainAmount);

	float GetCurrentStamina() const { return currentStamina; }

private:

	class ACharacter_Base* ownerCharacter;


	UPROPERTY(EditDefaultsOnly, Category = "VisibleStats")
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "VisibleStats")
	float currentHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "VisibleStats")
	int ActionPoints = 3;

	UPROPERTY(EditDefaultsOnly, Category = "VisibleStats")
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "VisibleStats")
	float currentStamina;

	UPROPERTY(VisibleAnywhere, Category = "Bonus Stats")
	float currentEquipmentWeight;

	UPROPERTY(EditDefaultsOnly, Category = "VisibleStats")
	float Speed = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "VisibleStats")
	float Strength = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "VisibleStats")
	float Dexterity = 30.f;

	UPROPERTY(VisibleAnywhere, Category = "Bonus Stats")
	float staminaReductionBonus;

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);
	void ChangeHealth(float amount);
};
