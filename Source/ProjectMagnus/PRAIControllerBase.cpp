// Fill out your copyright notice in the Description page of Project Settings.


#include "PRAIControllerBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerHQ.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.h"
#include "PlayerCharacter.h"
#include "PRAttributeSet.h"
#include "PRAbilitySystemComponent.h"

APRAIControllerBase::APRAIControllerBase()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->ConfigureSense(*DamageConfig);
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &APRAIControllerBase::OnPerceptionUpdated);
}

ETeamAttitude::Type APRAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* pawn = Cast<APawn>(&Other);

	if (!pawn)
		return ETeamAttitude::Neutral;

	auto pTI = Cast<IGenericTeamAgentInterface>(&Other);
	class IGenericTeamAgentInterface* bTI = Cast<IGenericTeamAgentInterface>(pawn->GetController());
	if (bTI == nullptr && pTI == nullptr)
		return ETeamAttitude::Neutral;


	FGenericTeamId otherID = NULL;
	if (bTI != nullptr)
	{
		otherID = bTI->GetGenericTeamId();
	}
	else if (pTI != nullptr)
	{
		otherID = pTI->GetGenericTeamId();
	}

	if (otherID == 20)
	{
		return ETeamAttitude::Neutral;
	}
	else if (otherID == TeamID)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}
}



void APRAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(behviorTree);
	ABaseEnemy* chara = Cast<ABaseEnemy>(GetPawn());
	AActor* PHQ = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerHQ::StaticClass());
	if (chara)
	{
		enemy = chara;
		TeamID = FGenericTeamId(chara->GetGenericTeamId());
		if (PHQ)
		{
			GetBlackboardComponent()->SetValueAsObject(PlayerHQKeyName, PHQ);
		}
	}
	else
	{
		APlayerCharacter* pChara = Cast<APlayerCharacter>(GetPawn());
		player = pChara;
		TeamID = 1;
	}
}

void APRAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (enemy)
	{
		GetBlackboardComponent()->SetValueAsFloat(StaminaPropertyKeyName, enemy->GetAttributeSet()->GetStamina());
	}

}

void APRAIControllerBase::SetStandingBehaviorTree()
{
	RunBehaviorTree(behviorTree);

}

void APRAIControllerBase::SetMovingBehaviorTree()
{
	RunBehaviorTree(movingAIBehaviorTree);

}



void APRAIControllerBase::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Seeing: %s"), *Actor->GetName());
		GetBlackboardComponent()->SetValueAsObject(TargetBlackboardKeyName, Actor);
		//SetFocus(Actor);
	}
	else
	{
		const FActorPerceptionInfo* perceptionInfo = PerceptionComp->GetActorInfo(*Actor);

		if (Actor == enemy)
		{
			if(enemy->GetAttributeSet()->GetHealth() == 0)
			GetBlackboardComponent()->ClearValue(TargetBlackboardKeyName);
		}

		if (Actor == player)
		{
			if (player->GetAttributeSet()->GetHealth() == 0)
				GetBlackboardComponent()->ClearValue(TargetBlackboardKeyName);
		}


		if (!perceptionInfo->HasAnyCurrentStimulus())
		{
			UE_LOG(LogTemp, Warning, TEXT("He gone: %s"), *Actor->GetName());
			GetBlackboardComponent()->ClearValue(TargetBlackboardKeyName);
			//ClearFocus(EAIFocusPriority::Gameplay);
			//GetBlackboardComponent()->SetValueAsVector(LastSeenKeyName, Actor->GetActorLocation());
		}


	}
}
