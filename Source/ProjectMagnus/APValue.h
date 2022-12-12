// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APValue.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UAPValue : public UUserWidget
{
	GENERATED_BODY()

private:
public:

	void SetMaxValue(float maxValue);
	void SetValue(float currentValue);

	UPROPERTY(meta = (BindWidget))
	class UImage* valueImage;

	UPROPERTY(EditDefaultsOnly, Category = "Value")
	FName ValueParamName;
	
};
