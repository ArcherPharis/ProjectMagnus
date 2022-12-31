// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "PRAIControllerBase.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API APRAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:
	FGenericTeamId TeamID;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	APRAIControllerBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAIPerceptionComponent* PerceptionComp;
	UPROPERTY()
	class UAISenseConfig_Damage* DamageConfig;
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetBlackboardKeyName;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* behviorTree;

	AActor* SensedActor;


	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
	
};
