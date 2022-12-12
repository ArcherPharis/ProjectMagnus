// Fill out your copyright notice in the Description page of Project Settings.


#include "APValue.h"
#include "Components/Image.h"


void UAPValue::SetMaxValue(float maxValue)
{
	valueImage->GetDynamicMaterial()->SetScalarParameterValue(ValueParamName, maxValue);
}

void UAPValue::SetValue(float currentValue)
{
	valueImage->GetDynamicMaterial()->SetScalarParameterValue(ValueParamName, currentValue);
}
