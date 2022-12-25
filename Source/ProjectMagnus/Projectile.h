// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Projectile.generated.h"

UCLASS()
class PROJECTMAGNUS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return mesh; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Project")
	void GetPowerForImpulse(float power);

	void SetProjectileSpeed(float speed, FVector direction);

	void SetEventTag(FGameplayTag tag);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	FGameplayTag TacticalDamageEventTag;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UCapsuleComponent* hitBox;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UProjectileMovementComponent* moveComp;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystem* explosionFX;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float explodeTime = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<class UGameplayEffect> effectToApply;

	void Explode();
};
