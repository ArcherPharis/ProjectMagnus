// Fill out your copyright notice in the Description page of Project Settings.


#include "Firearm.h"
#include "Kismet/GameplayStatics.h"
#include "Character_Base.h"
#include "PRAttributeSet.h"
#include "Components/CapsuleComponent.h"

AFirearm::AFirearm()
{
	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	firePoint->SetupAttachment(GetWeaponMesh());
}

void AFirearm::FirearmAim()
{
	wielderControlPercent = ((10 + 24) * Handleability) / 10;
	FHitResult result;
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector CameraEnd = cameraManager->GetCameraLocation() + cameraManager->GetActorForwardVector() * weaponRange;
	FCollisionShape cyl = FCollisionShape::MakeSphere(100);
	if (GetWorld()->LineTraceSingleByChannel(result, cameraManager->GetCameraLocation(), CameraEnd, ECC_GameTraceChannel1))
	{
		potentialActor = nullptr;
		targetedActor = result.GetActor();
		onForecastInfo.Broadcast(GetBulletsToKill(targetedActor), GetCurrentAmmo());

	}
	else if (GetWorld()->SweepSingleByChannel(result, cameraManager->GetCameraLocation(), CameraEnd, FQuat::Identity, ECC_GameTraceChannel1, cyl))
	{
		UE_LOG(LogTemp, Warning, TEXT("Looking at with sphere trace: %s"), *result.GetActor()->GetName());
		potentialActor = result.GetActor();
		PotentialActorResult(result);
	}
	else
	{
		onClearForecast.Broadcast();
	}


	GetWorldTimerManager().SetTimer(aimCastTimerHandle, this, &AFirearm::FirearmAim, 0.1f, true);
	
}

void AFirearm::StopFirearmAim()
{
	if(GetWorldTimerManager().IsTimerActive(aimCastTimerHandle))
	GetWorldTimerManager().ClearTimer(aimCastTimerHandle);
}

bool AFirearm::TryStopFiring()
{

	if (!GetPlayerWantsToStopFiring()) return false;
	onBeginAttackEvent.Broadcast();
	int randomRoll = FMath::RandRange(1, 150);

	if (wielderControlPercent <= 10)
	{
		wielderControlPercent = 15;
	}

	if (wielderControlPercent > randomRoll)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopped Firing! : %i"), randomRoll);
		SetPlayerWantsToStopFiring(false);
		//SetInAttackEvent(false);
		//APlayerController* pc = Cast<APlayerController>(GetWeaponOwner()->GetController());
		//pc->SetInputMode(FInputModeGameOnly());
		//GetWeaponOwner()->StopAiming();
		
		
		return true;
	}


	return false;
}

void AFirearm::Attack()
{
	if (GetCurrentAmmo() == 0) return;

	if (potentialActor)
	{
		if (GetWeaponOwner())
		RotateCharacter(GetWeaponOwner(), potentialActor);
		potentialActor = nullptr;
		return;
	}
	SetInAttackEvent(true);
	onClearForecast.Broadcast();
	APlayerController* pc = Cast<APlayerController>(GetWeaponOwner()->GetController());
	pc->SetInputMode(FInputModeUIOnly());
	onBeginAttackEvent.Broadcast();
	GetWeaponOwner()->PlayGunAttackClip();
	FTimerHandle delayShootHandle;
	GetWorldTimerManager().SetTimer(delayShootHandle, this, &AFirearm::BeginAttack, 0.5f, false);
	

}

FVector AFirearm::WeaponSpread(FVector EndPoint)
{	
	FVector finalResult;
	finalResult.X = EndPoint.X + FMath::FRandRange(minSpread, maxSpread);
	finalResult.Y = EndPoint.Y + FMath::FRandRange(minSpread, maxSpread);
	finalResult.Z = EndPoint.Z + FMath::FRandRange(minSpread, maxSpread);
	return finalResult;


}

FHitResult AFirearm::PotentialActorResult(FHitResult potResult)
{
	return potResult;

}

void AFirearm::WeaponFire()
{

	if (TryStopFiring()) return;
	if (GetCurrentAmmo() > 0)
	{		

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
	if (GetWorld()->LineTraceSingleByChannel(result, cameraManager->GetCameraLocation(), WeaponSpread(CameraEnd), ECC_GameTraceChannel1))
	{
		//TODO in the future, maybe a damagable interface? OR, simply if not damagable chara, use built in apply damnage.
		ACharacter_Base* damagableChara = Cast<ACharacter_Base>(result.GetActor());

		if (damagableChara)
		{
			
			damagableChara->GetAttributeSet()->SetHealth(FMath::Clamp(damagableChara->GetAttributeSet()->GetHealth() - damage, 0, damagableChara->GetAttributeSet()->GetMaxHealth()));
			if (damagableChara == targetedActor && damagableChara->GetIsDead())
			{
				UE_LOG(LogTemp, Warning, TEXT("Gun killed something"));
				GetWorldTimerManager().SetTimer(GunKilledHandle, this, &AFirearm::GunKilledTarget, 1.f, false);
				
			}
			//damagableChara->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf()
		}


		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, result.ImpactPoint);
		SpawnImpactEffects(result);
		//DrawDebugPoint(GetWorld(), result.Location, 10, FColor::Red, true, 2.f);

	}
	PlayWeaponSound(firePoint);
	ChangeCurrentAmmo(-1);

	if (GetCurrentAmmo() == 0)
	{
		//need to return the character back to their camera, after a small delay.
		//but we also need to check if the attack event is still true, just in case the
		//gun killed something on the last bullet.
		//SetInAttackEvent(false);		
		SetPlayerWantsToStopFiring(false);
	}
	onForecastInfo.Broadcast(GetBulletsToKill(targetedActor), GetCurrentAmmo());
	onWeaponUse.Broadcast(GetCurrentAmmo());
	GetWorldTimerManager().SetTimer(fireDelayTimer, this, &AFirearm::AfterFireCheck, 1 / fireRate);
}

}

void AFirearm::BeginAttack()
{
	WeaponFire();
}

void AFirearm::AfterFireCheck()
{
	canfire = true;
	OnStopAttack();
	//if (bFireButtonPressed && fireMode == Firetype::Automatic)
	WeaponFire();
		

}

int AFirearm::GetBulletsToKill(AActor* currentTarget)
{
	ACharacter_Base* character = Cast<ACharacter_Base>(currentTarget);
	if (character)
	{
		float currentTargetHealth = character->GetAttributeSet()->GetHealth();

		int shotsToKill =  FMath::CeilToInt(currentTargetHealth / damage);
		shotsToKill = FMath::Clamp(shotsToKill, 0, 100);
		return shotsToKill;
	}
	return 0;
	
}

void AFirearm::GunKilledTarget()
{


	ACharacter_Base* chr = Cast<ACharacter_Base>(targetedActor);

	if (chr == nullptr) return;
	GetWeaponOwner()->OnUnitDeath(chr);
	targetedActor = nullptr;
}
