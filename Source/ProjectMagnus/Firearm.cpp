// Fill out your copyright notice in the Description page of Project Settings.


#include "Firearm.h"
#include "Kismet/GameplayStatics.h"

AFirearm::AFirearm()
{
	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	firePoint->SetupAttachment(GetWeaponMesh());
}

void AFirearm::Attack()
{
	Super::Attack();
	if (canfire)
	{
		canfire = false;
	}
	else
	{
		return;
	}
	FHitResult result;
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	UGameplayStatics::SpawnEmitterAttached(muzzleEffect, firePoint);
	FVector CameraEnd = cameraManager->GetCameraLocation() + cameraManager->GetActorForwardVector() * weaponRange;
	OnAttack();
	if (GetWorld()->LineTraceSingleByChannel(result, cameraManager->GetCameraLocation(), WeaponSpread(CameraEnd), ECC_Camera))
	{
		UGameplayStatics::ApplyDamage(result.GetActor(), damage, nullptr, GetOwner(), nullptr);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, result.ImpactPoint);
		DrawDebugPoint(GetWorld(), result.Location, 10, FColor::Red, true, 2.f);

	}
	
	GetWorldTimerManager().SetTimer(fireDelayTimer, this, &AFirearm::AfterFireCheck, 1 / fireRate);

}

FVector AFirearm::WeaponSpread(FVector EndPoint)
{	 //firePoint->GetComponentLocation() + firePoint->GetForwardVector() * weaponRange;
	FVector finalResult;
	finalResult.X = EndPoint.X + FMath::FRandRange(minSpread, maxSpread);
	finalResult.Y = EndPoint.Y + FMath::FRandRange(minSpread, maxSpread);
	finalResult.Z = EndPoint.Z + FMath::FRandRange(minSpread, maxSpread);
	return finalResult;


}

void AFirearm::WeaponFire()
{


}

void AFirearm::AfterFireCheck()
{
	canfire = true;
	OnStopAttack();
	if (bFireButtonPressed && fireMode == Firetype::Automatic)
	{
		Attack();
	}
}
