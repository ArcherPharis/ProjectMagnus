// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTMAGNUS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnEquip(USkeletalMeshComponent* ownerMesh);

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USceneComponent* root;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USkeletalMeshComponent* mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName socketName;
};
