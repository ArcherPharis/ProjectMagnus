// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyCaptureBased.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "PRAIControllerBase.h"
#include "BaseEnemy.h"

UBTT_EnemyCaptureBased::UBTT_EnemyCaptureBased()
{
	NodeName = "Capture Player Base";
}

EBTNodeResult::Type UBTT_EnemyCaptureBased::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	ABaseEnemy* enemy = AIC->GetPawn<ABaseEnemy>();


	if (enemy)
	{
		enemy->EndGame();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
