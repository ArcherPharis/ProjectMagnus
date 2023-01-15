// Fill out your copyright notice in the Description page of Project Settings.


#include "Firearm.h"
#include "Kismet/GameplayStatics.h"
#include "Character_Base.h"
#include "BaseEnemy.h"
#include "PRAttributeSet.h"
#include "MPlayerController.h"
#include "PMGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "PRAbilitySystemComponent.h"

AFirearm::AFirearm()
{
	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	firePoint->SetupAttachment(GetWeaponMesh());
}

void AFirearm::FirearmAim()
{
	if (GetInAttackEvent()) return;

	FHitResult result;
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector CameraEnd = cameraManager->GetCameraLocation() + cameraManager->GetActorForwardVector() * weaponRange;
	FCollisionShape cyl = FCollisionShape::MakeSphere(100);
	if (GetWorld()->LineTraceSingleByChannel(result, cameraManager->GetCameraLocation(), CameraEnd, ECC_GameTraceChannel1))
	{
		potentialActor = nullptr;
		targetedActor = result.GetActor();

	}
	else if (GetWorld()->SweepSingleByChannel(result, cameraManager->GetCameraLocation(), CameraEnd, FQuat::Identity, ECC_GameTraceChannel1, cyl))
	{
		UE_LOG(LogTemp, Warning, TEXT("Looking at with sphere trace: %s"), *result.GetActor()->GetName());
		potentialActor = result.GetActor();
		PotentialActorResult(result);
	}



	GetWorldTimerManager().SetTimer(aimCastTimerHandle, this, &AFirearm::FirearmAim, 0.1f, true);
	
}

void AFirearm::StopFirearmAim()
{
	if(GetWorldTimerManager().IsTimerActive(aimCastTimerHandle))
	GetWorldTimerManager().ClearTimer(aimCastTimerHandle);
}


void AFirearm::ReloadWeapon()
{
	if (GetCurrentAmmo() < GetMaxAmmo() && GetAmmoReserves() != 0)
	{
		int result = GetMaxAmmo() - GetCurrentAmmo();

		if (result <= GetAmmoReserves())
		{
			SetCurrentAmmo(GetCurrentAmmo() + result);
			SetAmmoReserves(GetAmmoReserves() - result);
			onWeaponUse.Broadcast(GetCurrentAmmo(), GetAmmoReserves());
		}
		else
		{
			ChangeAmmoReserves(GetAmmoReserves());
			SetAmmoReserves(0);
			onWeaponUse.Broadcast(GetCurrentAmmo(), GetAmmoReserves());
		}
	}
}




void AFirearm::DecrementAmmo()
{
	ChangeCurrentAmmo(-1);
	if (GetCurrentAmmo() == 0)
	{
		Reload();
	}
}

void AFirearm::Attack()
{
	if (GetCurrentAmmo() == 0)
	{

		if (GetAmmoReserves() > 0)
		{
			GetWeaponOwner()->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(reloadTag);
		}
		return;
	}
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
		UPRAbilitySystemComponent* ASC = result.GetActor()->FindComponentByClass<UPRAbilitySystemComponent>();
		if (ASC)
		{
			FGameplayEffectContextHandle handle;
			ASC->ApplyGameplayEffectToSelf(damageEffect.GetDefaultObject(), -1, handle);
			UGameplayStatics::PlaySound2D(this, hitMarkerSound, 1.5f);
			UGameplayStatics::PlaySound2D(this, impactSound, 0.5f);

			ACharacter_Base* damagableChara = Cast<ACharacter_Base>(result.GetActor());


			if (damagableChara && damagableChara->GetAttributeSet()->GetHealth() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Gun killed something"));

			}


		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, result.ImpactPoint);
		SpawnImpactEffects(result);

	}

	PlayWeaponSound(firePoint);
	GetWeaponOwner()->GetMesh()->GetAnimInstance()->Montage_Play(GetAttackMontage());
	ChangeCurrentAmmo(-1);
	onWeaponUse.Broadcast(GetCurrentAmmo(), GetAmmoReserves());
	GetWorldTimerManager().SetTimer(fireDelayTimer, this, &AFirearm::AfterFireCheck, 1 / GetFireRate());
}

	
FHitResult AFirearm::PotentialActorResult(FHitResult potResult)
{
	return potResult;

}



FVector AFirearm::WeaponSpread(FVector EndPoint)
{	
	FVector finalResult;
	finalResult.X = EndPoint.X + FMath::FRandRange(minSpread, maxSpread);
	finalResult.Y = EndPoint.Y + FMath::FRandRange(minSpread, maxSpread);
	finalResult.Z = EndPoint.Z + FMath::FRandRange(minSpread, maxSpread);
	return finalResult;


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


