// Fill out your copyright notice in the Description page of Project Settings.
#include "ValueGauge.h"
#include "Components/Image.h"

void UValueGauge::SetValue(float newValue, float max)
{
	valueImage->GetDynamicMaterial()->SetScalarParameterValue(ValueParamName, newValue / max);
}

void UValueGauge::SetHealthRange(float newRange)
{
	valueImage->GetDynamicMaterial()->SetScalarParameterValue(RangeParamName, newRange / 100);
}

void UValueGauge::SetStamRange(float newRange)
{
	valueImage->GetDynamicMaterial()->SetScalarParameterValue(RangeParamName, newRange / 250);
}
