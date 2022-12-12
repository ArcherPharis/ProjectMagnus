// Fill out your copyright notice in the Description page of Project Settings.


#include "PMGameModeBase.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "InGameUI.h"


void APMGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void APMGameModeBase::GetAllSpawnLocations()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), spawnPoints);
	
}

void APMGameModeBase::SpawnInitialUnits(UInGameUI* aUI)
{
	GetAllSpawnLocations();

	for (TSubclassOf<APlayerCharacter> character : deployablePlayerUnits)
	{
		APlayerCharacter* initCharacter =  GetWorld()->SpawnActor<APlayerCharacter>(character);
		currentPlayerUnits.Add(initCharacter);
		aUI->NewUnitGiven(initCharacter);
		
	}
	
	for (APlayerCharacter* player : currentPlayerUnits)
	{
		player->GiveEquipment();

	}

	if (numberOfUnitsAllowedForChapter == spawnPoints.Num())
	{
		if (currentPlayerUnits.Num() > 0 && currentPlayerUnits.Num() <= spawnPoints.Num())
		{
			for (int i = 0; i < currentPlayerUnits.Num(); i++)
			{
				currentPlayerUnits[i]->SetActorLocation(spawnPoints[i]->GetActorLocation());
				currentPlayerUnits[i]->SetActorRotation(spawnPoints[i]->GetActorRotation());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hey! You either have no deployable units or more units than spawn points!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hey! You don't have the required amount of spawn points!"));
	}
	//todo, we also put up each initial ui on the list
}
