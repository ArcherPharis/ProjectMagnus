// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EndTurn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UBTT_EndTurn : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_EndTurn();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
