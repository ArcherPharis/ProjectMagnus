// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TargetPoint.h"
#include "PMGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API APMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void SpawnInitialUnits(class UInGameUI* aUI);

private:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void GetAllSpawnLocations();
	

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	TArray<TSubclassOf<class APlayerCharacter>> deployablePlayerUnits;

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	TArray<AActor*> spawnPoints;
	
	TArray<APlayerCharacter*> currentPlayerUnits;

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	int numberOfUnitsAllowedForChapter = 3;
};
