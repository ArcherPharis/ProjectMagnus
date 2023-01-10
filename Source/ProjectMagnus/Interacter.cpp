// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacter.h"
#include "InteractableComponent.h"
#include "Character_Base.h"

UInteracter::UInteracter()
{
	PrimaryComponentTick.bCanEverTick = true;
	OnComponentBeginOverlap.AddDynamic(this, &UInteracter::OnTouch);
	OnComponentEndOverlap.AddDynamic(this, &UInteracter::OnLeave);
}

void UInteracter::BeginPlay()
{
	Super::BeginPlay();
	owningPlayer = Cast<ACharacter_Base>(GetOwner());
}

void UInteracter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteracter::InitiateInteract()
{
	if (interactable)
	{
		ACharacter_Base* charact = Cast<ACharacter_Base>(GetOwner());
		interactable->Interact(charact);

	}
}

void UInteracter::OnTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	interactable = Cast<UInteractableComponent>(OtherActor->GetComponentByClass(UInteractableComponent::StaticClass()));
	owningPlayer->onDisplayTip.Broadcast("Press E to interact");
	owningPlayer->ToggleMessageOffWTimer(2);
}

void UInteracter::OnLeave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (interactable)
	{
		interactable = nullptr;
	}
	owningPlayer->onDisplayTip.Broadcast("");
}
