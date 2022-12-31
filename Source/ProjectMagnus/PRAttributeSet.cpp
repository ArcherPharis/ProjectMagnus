// Fill out your copyright notice in the Description page of Project Settings.


#include "PRAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

void UPRAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }

    if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
    }

    if (Data.EvaluatedData.Attribute == GetActionPointsAttribute())
    {
        SetActionPoints(FMath::Clamp(GetActionPoints(), 0.f, GetMaxActionPoints()));
    }

    if (Data.EvaluatedData.Attribute == GetExperiencePointsAttribute())
    {
        SetExperiencePoints(FMath::Clamp(GetExperiencePoints(), 0.f, GetMaxExperiencePoints()));
    }

    if (Data.EvaluatedData.Attribute == GetStrengthAttribute())
    {
        SetStrength(FMath::Clamp(GetStrength(), 0.f, GetMaxStrength()));
    }

    if (Data.EvaluatedData.Attribute == GetEnduranceAttribute())
    {
        SetEndurance(FMath::Clamp(GetEndurance(), 0.f, GetMaxEndurance()));
    }

    if (Data.EvaluatedData.Attribute == GetAgilityAttribute())
    {
        SetAgility(FMath::Clamp(GetAgility(), 0.f, GetMaxAgility()));
    }

    if (Data.EvaluatedData.Attribute == GetDexterityAttribute())
    {
        SetDexterity(FMath::Clamp(GetDexterity(), 0.f, GetMaxDexterity()));
    }

    if (Data.EvaluatedData.Attribute == GetLevelAttribute())
    {
        SetLevel(FMath::Clamp(GetLevel(), 0.f, GetMaxLevel()));
    }

    if (Data.EvaluatedData.Attribute == GetArmorAttribute())
    {
        SetArmor(FMath::Clamp(GetArmor(), 0.f, GetMaxArmor()));
    }
}


