// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UnitListEntry.generated.h"




/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UUnitListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UnitButtonPressed();

	UFUNCTION()
	void SetUnitEntryAP(float value);


private:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject);

	UPROPERTY(meta = (BindWidget))
	class UImage* unitPortrait;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* unitName;
	UPROPERTY(meta = (BindWidget))
	class UButton* unitSelectButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* APCount;
	
};
