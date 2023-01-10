// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EndTurn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "PRAIControllerBase.h"
#include "BaseEnemy.h"

UBTT_EndTurn::UBTT_EndTurn()
{
	NodeName = "End Turn";
}

EBTNodeResult::Type UBTT_EndTurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	ABaseEnemy* enemy = AIC->GetPawn<ABaseEnemy>();


	if (enemy)
	{
		enemy->EndTurn();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
