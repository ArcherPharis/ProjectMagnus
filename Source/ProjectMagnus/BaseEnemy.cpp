// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PMGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PRAbilitySystemComponent.h"
#include "PRGameplayAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character_Base.h"
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
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &ABaseEnemy::CharacterDied);
	ApplyInitialEffect();
	
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

void ABaseEnemy::SetKiller(AActor* killer)
{
	Killer = killer;
	APMGameModeBase* gm = Cast<APMGameModeBase>(UGameplayStatics::GetGameMode(this));
	gm->AddKilledUnits(this);
}

void ABaseEnemy::AwardKillerWithEXP()
{
	ACharacter_Base* awardee = Cast<ACharacter_Base>(Killer);
	if (awardee)
	{
		FGameplayEffectContextHandle handle;
		awardee->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(experienceWorthEffect.GetDefaultObject(), -1, handle);
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
		//isDead = true;
		SetActorEnableCollision(false);
	}
}



