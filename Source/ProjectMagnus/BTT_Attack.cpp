// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "PRAIControllerBase.h"
#include "UnitAIInterface.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	IUnitAIInterface* AIInterface = AIC->GetPawn<IUnitAIInterface>();

	if (AIInterface)
	{
		AIInterface->Attack();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
