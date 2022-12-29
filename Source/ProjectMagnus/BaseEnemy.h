// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyAbilityAdded, UPRGameplayAbilityBase*, newAbility);

class UPRGameplayAbilityBase;

UCLASS()
class PROJECTMAGNUS_API ABaseEnemy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();
	void ApplyInitialEffect();

	FOnEnemyAbilityAdded onAbilityAdded;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	FORCEINLINE class UPRAttributeSet* GetAttributeSet() const { return attributeSet; }
	FName GetUnitName() const { return unitName; }

	TSubclassOf<APawn> GetDeathPawnClass() const { return deathPawnClass; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbilities")
	void ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* GetKiller() const { return Killer; }

	UFUNCTION(BlueprintCallable, Category = "BaseEnemy")
	void SetKiller(AActor* killer);
	void AwardKillerWithEXP();

private:
	void GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID = -1, bool broadCast = false);
	void CharacterDied(const FOnAttributeChangeData& AttributeData);

	UPROPERTY()
	class UPRAbilitySystemComponent* abilitySystemComp;

	UPROPERTY()
	UPRAttributeSet* attributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TArray<TSubclassOf<class UGameplayEffect>> InitialEffects;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TArray<TSubclassOf<UGameplayAbility>> InitialAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Personal Info")
	FName unitName;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Info")
	TSubclassOf<UGameplayEffect> experienceWorthEffect;

	UPROPERTY(EditDefaultsOnly, Category = "PossessablePawns")
	TSubclassOf<APawn> deathPawnClass;
	UPROPERTY(EditDefaultsOnly, Category = "PossessablePawns")
	UAnimMontage* onDeadMontage;

	AActor* Killer;
};
