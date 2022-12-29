// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PRAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UPRAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UPROPERTY()
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, Health);

	UPROPERTY()
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxHealth);

	UPROPERTY()
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, Stamina);

	UPROPERTY()
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxStamina);

	UPROPERTY()
	FGameplayAttributeData ActionPoints;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, ActionPoints);

	UPROPERTY()
	FGameplayAttributeData MaxActionPoints;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxActionPoints);

	UPROPERTY()
	FGameplayAttributeData ExperiencePoints;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, ExperiencePoints);

	UPROPERTY()
	FGameplayAttributeData MaxExperiencePoints;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxExperiencePoints);

	UPROPERTY()
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, Strength);

	UPROPERTY()
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxStrength);

	UPROPERTY()
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, Endurance);

	UPROPERTY()
	FGameplayAttributeData MaxEndurance;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxEndurance);

	UPROPERTY()
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, Agility);

	UPROPERTY()
	FGameplayAttributeData MaxAgility;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxAgility);

	UPROPERTY()
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, Dexterity);

	UPROPERTY()
	FGameplayAttributeData MaxDexterity;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxDexterity);

	UPROPERTY()
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, Level);

	UPROPERTY()
		FGameplayAttributeData MaxLevel;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxLevel);

};
	

