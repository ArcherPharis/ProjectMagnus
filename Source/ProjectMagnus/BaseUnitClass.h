// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseUnitClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTMAGNUS_API UBaseUnitClass : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseUnitClass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UPRAttributeSet* GetOwnersAttributeSet() const { return attributeSet; }
	TSubclassOf<class UGameplayAbility> GetClassFieldAbility() const { return fieldAbility; }
	

private:
	UPRAttributeSet* attributeSet;


	UPROPERTY(EditDefaultsOnly, Category = "Field Ability")
	TSubclassOf<UGameplayAbility> fieldAbility;
		
};
