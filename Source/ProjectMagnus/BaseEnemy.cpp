// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PMGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "PRAIControllerBase.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "PRAbilitySystemComponent.h"
#include "PRGameplayAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character_Base.h"
#include "Weapon.h"
#include "PRAttributeSet.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	abilitySystemComp = CreateDefaultSubobject<UPRAbilitySystemComponent>(TEXT("Ability System Comp"));
	attributeSet = CreateDefaultSubobject<UPRAttributeSet>(TEXT("Attribute Set"));

}

void ABaseEnemy::ApplyInitialEffect()
{
	for (auto effect : InitialEffects)
	{
		ApplyEffectToSelf(effect);
	}
}

UAbilitySystemComponent* ABaseEnemy::GetAbilitySystemComponent() const
{
	return abilitySystemComp;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	gameMode = Cast<APMGameModeBase>(UGameplayStatics::GetGameMode(this));
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &ABaseEnemy::CharacterDied);
	ApplyInitialEffect();
	SpawnWeapon();
	aiController = Cast<APRAIControllerBase>(GetOwner());
	
}

void ABaseEnemy::ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply)
{
	FGameplayEffectSpecHandle InitialSpec = abilitySystemComp->MakeOutgoingSpec(effectToApply, -1, abilitySystemComp->MakeEffectContext());
	abilitySystemComp->ApplyGameplayEffectSpecToSelf(*InitialSpec.Data);
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEnemy::SetLogicEnabled(bool bIsLogicEnabled)
{

	if (attributeSet->GetHealth() == 0 && bIsLogicEnabled)
	{
		return;
	}
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		UBrainComponent* braincomp = AIC->GetBrainComponent();
		if (braincomp)
		{
			if (GetAttributeSet()->GetHealth() > 0)
			{
				if (bIsLogicEnabled)
				{
					braincomp->StartLogic();
				}
				else
				{
					braincomp->StopLogic("Dead");
				}
			}

		}
	}
}

void ABaseEnemy::Attack()
{
	if (currentWeapon != nullptr)
	{
		currentWeapon->AttackAI();
	}
}

void ABaseEnemy::BeginEnemyTurn()
{
	GetAttributeSet()->SetStamina(GetAttributeSet()->GetMaxStamina());
	ApplyEffectToSelf(staminaDrainEffect);
	GetCurrentWeapon()->SetCurrentAmmo(GetCurrentWeapon()->GetMaxAmmo());
	aiController->SetMovingBehaviorTree();
}

void ABaseEnemy::EnemyTurnAttack()
{
	GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(staminaEffectTag);
	GetCurrentWeapon()->AttackAI();

	if (GetCurrentWeapon()->IsWeaponEmpty())
	{
		EndTurn();
	}

}

void ABaseEnemy::RotateTowardsAttackerAndRetaliate(AActor* attacker)
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		AIC->SetFocus(attacker);
	}
}

void ABaseEnemy::EndTurn()
{
	GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(staminaEffectTag);
	aiController->SetStandingBehaviorTree();
	SetLogicEnabled(false);
	gameMode->CycleThroughEnemyUnitTurns(this);
}

void ABaseEnemy::EndGame()
{
	APlayerController* cont = UGameplayStatics::GetPlayerController(this, 0);
	gameMode->ToggleEnemyLogic(false);
	endGameScreen = CreateWidget<UUserWidget>(cont, endGameScreenClass);
	endGameScreen->AddToViewport();
	UGameplayStatics::SetGamePaused(this, true);
}

void ABaseEnemy::SetKiller(AActor* killer)
{
	if (attributeSet->GetHealth() == 0)
	{
		Killer = killer;
		APMGameModeBase* gm = Cast<APMGameModeBase>(UGameplayStatics::GetGameMode(this));
		gm->AddKilledUnits(this);
	}
}

void ABaseEnemy::AwardKillerWithEXP()
{
	ACharacter_Base* awardee = Cast<ACharacter_Base>(Killer);
	
	if (awardee)
	{
		FGameplayEffectContextHandle handle;
		if (gameMode->IsEnemyPhase())
		{
			FName name = awardee->GetUnitName();
			FString string = "Unit has gained experience: " + name.ToString();
			awardee->onDisplayTip.Broadcast(string);
			awardee->ToggleMessageOffWTimer(3.5f);
		}
		awardee->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(experienceWorthEffect.GetDefaultObject(), -1, handle);
	}
}

void ABaseEnemy::SetKillerOnly(AActor* killer)
{
	Killer = killer;
}

void ABaseEnemy::SpawnWeapon()
{
	if (weaponClass)
	{
		AWeapon* weapon;
		weapon = GetWorld()->SpawnActor<AWeapon>(weaponClass);
		weapon->SetOwner(this);
		weapon->OnEquip(GetMesh());
		currentWeapon = weapon;

	}
}

void ABaseEnemy::GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID, bool broadCast)
{
	FGameplayAbilitySpecHandle AbilitySpecHandle = abilitySystemComp->GiveAbility(FGameplayAbilitySpec(newAbility, 1, inputID));

	if (broadCast)
	{
		onAbilityAdded.Broadcast(Cast<UPRGameplayAbilityBase>(GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability));
	}
}

void ABaseEnemy::CharacterDied(const FOnAttributeChangeData& AttributeData)
{


	if (AttributeData.NewValue == 0)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(onDeadMontage);
		GetCurrentWeapon()->SetActorHiddenInGame(true);
		GetCurrentWeapon()->Destroy();
		//isDead = true; 
		SetLogicEnabled(false);
		SetActorEnableCollision(false);
		

	}
}




