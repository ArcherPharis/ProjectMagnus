// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyTurnAttack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "PRAIControllerBase.h"
#include "UnitAIInterface.h"

UBTT_EnemyTurnAttack::UBTT_EnemyTurnAttack()
{
	NodeName = "EnemyOnlyAttack";
}

EBTNodeResult::Type UBTT_EnemyTurnAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	IUnitAIInterface* AIInterface = AIC->GetPawn<IUnitAIInterface>();

	if (AIInterface)
	{
		AIInterface->EnemyTurnAttack();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
