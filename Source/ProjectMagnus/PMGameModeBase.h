// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TargetPoint.h"
#include "PMGameModeBase.generated.h"

UENUM()
enum Phase
{
	PlayerPhase UMETA(DisplayName = "Player Phase"),
	EnemyPhase UMETA(DisplayName = "EnemyPhase"),
};

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API APMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	bool IsPlayerPhase();
	bool IsEnemyPhase();


	void SpawnInitialUnits(class UInGameUI* aUI);

	void CycleThroughEnemyUnitTurns(class ABaseEnemy* enemyDoneWithTurn);
	void BeginEnemyTurn();

	class APlayerCharacter* GetCurrentDeployedUnit() const { return currentlyDeployedPlayerUnit; }
	void SetCurrentDeployedPlayerUnit(APlayerCharacter* deployed);

	void AddDownedUnits(class ACharacter_Base* downedUnit);
	void AddDeadEnemyUnit(ABaseEnemy* killedUnit);
	void AddKilledUnits(ABaseEnemy* killedUnit);

	void ReturnToTacticsPawn();

	void ToggleEnemyLogic(bool bStopLogic);
	void TogglePlayerLogic(bool bStopLogic);
	void RemoveEnemyFromList(ABaseEnemy* enemyToRemove);
	

	APawn* GetTacticsPawn() const { return TacticsPawn; }

private:

	UPROPERTY(EditDefaultsOnly, Category = "Current Game Phase")
	TEnumAsByte<Phase> currentPhase;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void GetAllSpawnLocations();
	void PossessTacticsActor();

	class APlayerCharacter* CharacterToReturnTo;

	APlayerCharacter* currentlyDeployedPlayerUnit;
	

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	TArray<TSubclassOf< APlayerCharacter>> deployablePlayerUnits;

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	TArray<AActor*> spawnPoints;

	APawn* TacticsPawn;
	
	TArray<APlayerCharacter*> currentPlayerUnits;
	TArray<ABaseEnemy*> currentEnemyUnits;
	TArray<ABaseEnemy*> movingEnemyUnits;

	TArray<ACharacter_Base*> downedPlayerUnits;
	TArray<ABaseEnemy*> killedEnemyUnits;
	class UInGameUI* ui;

	UPROPERTY(EditDefaultsOnly, Category = "Deployable Units")
	int numberOfUnitsAllowedForChapter = 3;

	FTimerHandle killedOrDownedUnitHandle;
	void ShowDownedDeadUnits();

	FTimerHandle toNextEnemyTurnHandle;
	void GoToNextEnemy();
	void EndEnemyTurn();
};
