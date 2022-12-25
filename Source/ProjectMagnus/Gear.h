// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Gear.generated.h"

UCLASS()
class PROJECTMAGNUS_API AGear : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseGear();

	void AttachToCharacterMesh(USkeletalMeshComponent* characterMesh);
	void AttatchToCharacterHand(USkeletalMeshComponent* characterMesh);
	FORCEINLINE int GetUses() const { return Uses; }
	void SetUses(int value);


	FORCEINLINE UAnimMontage* GetBeginUseMontage() const { return beginUseMontage; }
	FORCEINLINE UAnimMontage* GetConfirmUseMontage() const { return confirmUseMontage; }


	TSubclassOf<class UGameplayEffect> GetGameplayEffect() const { return effectToApply; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	UStaticMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	UTexture2D* GearIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<UGameplayEffect> effectToApply;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	UAnimMontage* beginUseMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	UAnimMontage* confirmUseMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	FName onCharacterMeshSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	FName inCharacterHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	int Uses = 1;

};
