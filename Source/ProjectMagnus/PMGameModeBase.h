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

	void AddDownedUnits(class ACharacter_Base* downedUnit);
	void AddKilledUnits(class ABaseEnemy* killedUnit);

	void ReturnToTacticsPawn();

	void ToggleEnemyLogic(bool bStopLogic);
	void RemoveEnemyFromList(ABaseEnemy* enemyToRemove);
	

	APawn* GetTacticsPawn() const { return TacticsPawn; }

private:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void GetAllSpawnLocations();
	void PossessTacticsActor();
	class APlayerCharacter* CharacterToReturnTo;
	

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	TArray<TSubclassOf< APlayerCharacter>> deployablePlayerUnits;

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	TArray<AActor*> spawnPoints;

	APawn* TacticsPawn;
	
	TArray<APlayerCharacter*> currentPlayerUnits;
	TArray<ABaseEnemy*> currentEnemyUnits;

	TArray<ACharacter_Base*> downedPlayerUnits;
	TArray<ABaseEnemy*> killedEnemyUnits;
	class UInGameUI* ui;

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	int numberOfUnitsAllowedForChapter = 3;

	FTimerHandle killedOrDownedUnitHandle;
	void ShowDownedDeadUnits();
};
