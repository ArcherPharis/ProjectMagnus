// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerHQ.generated.h"

UCLASS()
class PROJECTMAGNUS_API APlayerHQ : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerHQ();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "HeadQuarters")
	USceneComponent* rootComp;

	UPROPERTY(EditAnywhere, Category = "HeadQuarters")
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "HeadQuarters")
	class USphereComponent* hitSphere;

};
