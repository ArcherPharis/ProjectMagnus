// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_ExecGun.h"
#include "PRAttributeSet.h"

struct DamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	DamageCapture()
	{
		//for melee damage, Endurance would be a Source, since it isn't being affected but being used.
		//source is...the person who is applying the effect, their stat. target is who its being applied to.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPRAttributeSet, Armor, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPRAttributeSet, Health, Target, true);

	}

};

static DamageCapture& GetDamageCapture()
{
	static DamageCapture DamageCaptureVar;
	return DamageCaptureVar;
}


UGE_ExecGun::UGE_ExecGun()
{
	RelevantAttributesToCapture.Add(GetDamageCapture().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().HealthDef);
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Unit.calc.damage.gun"));

}

void UGE_ExecGun::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecParams, FGameplayEffectCustomExecutionOutput& ExecOutputs) const
{
	//calc variables
	float OutHealth = 0.0f;
	float OutArmor = 0.0f;

	//capture variables
	float ArmorMagnitude = 0.0f;
	float HealthMagnitude = 0.0f;
	//this would be our source data, as in an attribute. strength stat for example.
	float GunDamage = 0.0f;

	ExecParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().ArmorDef, FAggregatorEvaluateParameters(), ArmorMagnitude);
	ExecParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().HealthDef, FAggregatorEvaluateParameters(), HealthMagnitude);
	ExecParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Unit.calc.damage.gun"), FAggregatorEvaluateParameters(), GunDamage);

	//start calculation
	//the armor takes half the damage the gun deals, and in turn only half the damage is applied
	//to the unit. if the armor ever drops to 0, then the armor is gone and the unit takes full
	//damage.

	OutArmor = ArmorMagnitude - GunDamage/2;
	ExecOutputs.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().ArmorProperty, EGameplayModOp::Override, OutArmor));

	if (OutArmor > 0)
	{
		OutHealth = HealthMagnitude - GunDamage / 2;
		ExecOutputs.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().HealthProperty, EGameplayModOp::Override, OutHealth));

	}
	else
	{
		OutHealth = HealthMagnitude - GunDamage;
		ExecOutputs.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().HealthProperty, EGameplayModOp::Override, OutHealth));

	}
}
