// Fill out your copyright notice in the Description page of Project Settings.


#include "PMGameModeBase.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "InGameUI.h"

void APMGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void APMGameModeBase::SpawnInitialUnits(UInGameUI* aUI)
{
	for (TSubclassOf<APlayerCharacter> character : deployablePlayerUnits)
	{
		APlayerCharacter* initCharacter =  GetWorld()->SpawnActor<APlayerCharacter>(character);
		currentPlayerUnits.Add(initCharacter);
		aUI->NewUnitGiven(initCharacter);
	}

	UGameplayStatics::GetPlayerController(this, 0)->Possess(currentPlayerUnits[0]);
	//todo, we also put up each initial ui on the list
}
