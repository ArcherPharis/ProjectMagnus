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

	wielderControlPercent = ((10 + 24) * Handleability) / 10; //(level + dexterity)
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
		return true;
	}


	return false;
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

void AFirearm::Reload()
{
	Super::Reload();
	if (IsReloading()) return;

	ABaseEnemy* enemy = Cast<ABaseEnemy>(GetOwner());
	if (enemy)
	{
		float reloadDuration = enemy->GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
		GetWorldTimerManager().SetTimer(reloadHandle, this, &AFirearm::ReloadTimePoint, reloadDuration, false);
	}


	
}

bool AFirearm::IsReloading()
{
	ABaseEnemy* enemy = Cast<ABaseEnemy>(GetOwner());
	return enemy->GetMesh()->GetAnimInstance()->Montage_IsPlaying(ReloadMontage);
}

void AFirearm::ReloadTimePoint()
{
	SetCurrentAmmo(GetMaxAmmo());
}

void AFirearm::DecrementAmmo()
{
	ChangeCurrentAmmo(-1);
	UE_LOG(LogTemp, Warning, TEXT("%i"), GetCurrentAmmo());
	if (GetCurrentAmmo() == 0)
	{
		Reload();
	}
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

void AFirearm::AttackPointAnimNotify()
{
	if (IsReloading() || GetCurrentAmmo() == 0) return;

	Super::AttackPointAnimNotify();

	FHitResult result;
	FVector ownerViewLoc;
	FRotator ownerViewRot;
	GetOwner()->GetActorEyesViewPoint(ownerViewLoc, ownerViewRot);
	FVector Start = firePoint->GetComponentLocation();
	if (GetWorld()->LineTraceSingleByChannel(result, Start, WeaponSpread(ownerViewLoc + ownerViewRot.Vector() * weaponRange) , ECC_Camera))
	{
		UGameplayStatics::PlaySoundAtLocation(this, impactSound, result.ImpactPoint, 0.1f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, result.ImpactPoint);
		UPRAbilitySystemComponent* ASC = result.GetActor()->FindComponentByClass<UPRAbilitySystemComponent>();
		if (ASC)
		{
			FGameplayEffectContextHandle handle;
			ASC->ApplyGameplayEffectToSelf(damageEffect.GetDefaultObject(), -1, handle);
		}
		
	}
	DecrementAmmo();
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
		
		UPRAbilitySystemComponent* ASC = result.GetActor()->FindComponentByClass<UPRAbilitySystemComponent>();
		if (ASC)
		{
			
			//damagableChara->GetAttributeSet()->SetHealth(FMath::Clamp(damagableChara->GetAttributeSet()->GetHealth() - damage, 0, damagableChara->GetAttributeSet()->GetMaxHealth()));
			FGameplayEffectContextHandle handle;
			ASC->ApplyGameplayEffectToSelf(damageEffect.GetDefaultObject(), -1, handle);
			UGameplayStatics::PlaySound2D(this, hitMarkerSound, 1.5f);
			UGameplayStatics::PlaySound2D(this, impactSound, 0.5f);
			
			ACharacter_Base* damagableChara = Cast<ACharacter_Base>(result.GetActor());
			

			if (damagableChara && damagableChara->GetAttributeSet()->GetHealth() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Gun killed something"));
				killedSomething = true;
				//killedEnemies.Add(damagableChara);
				//GetWorldTimerManager().SetTimer(GunKilledHandle, this, &AFirearm::GunKilledTarget, 1.f, false);
				
			}
			else if (!damagableChara)
			{
				ABaseEnemy* damagableEnemy = Cast<ABaseEnemy>(result.GetActor());
				if (engagedEnemy == nullptr)
				{
					engagedEnemy = damagableEnemy;
				}
				
				if (damagableEnemy && damagableEnemy->GetAttributeSet()->GetHealth() == 0)
				{
					killedSomething = true;
					damagableEnemy->SetKiller(GetWeaponOwner());
					UE_LOG(LogTemp, Warning, TEXT("Gun killed enemy"));
				}

			}
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, result.ImpactPoint);
		SpawnImpactEffects(result);
		//DrawDebugPoint(GetWorld(), result.Location, 10, FColor::Red, true, 2.f);

	}
	PlayWeaponSound(firePoint);
	GetWeaponOwner()->GetMesh()->GetAnimInstance()->Montage_Play(GetAttackMontage());
	ChangeCurrentAmmo(-1);
	onWeaponUse.Broadcast(GetCurrentAmmo(), GetAmmoReserves());
	GetWorldTimerManager().SetTimer(fireDelayTimer, this, &AFirearm::AfterFireCheck, 1 / GetFireRate());
}
	else
	{
		if (GetInAttackEvent() && !killedSomething)
		{

			if (engagedEnemy->GetAttributeSet()->GetHealth() > 0)
			{

				FTimerHandle delayFireHandle;
				GetWorldTimerManager().SetTimer(delayFireHandle, this, &AFirearm::GoToRetaliate, 1.5f);



			}


			
		}
	}

}

void AFirearm::ReturnToInit()
{
	APlayerController* cont = UGameplayStatics::GetPlayerController(this, 0);
	cont->SetInputMode(FInputModeGameOnly());
	cont->SetViewTargetWithBlend(GetWeaponOwner(), 0.5f);
	SetInAttackEvent(false);
	GetWeaponOwner()->StopAiming();
	GetWeaponOwner()->StopAimMovement();
	killedSomething = false;
	engagedEnemy = nullptr;
}

void AFirearm::GoToRetaliate()
{
	engagedEnemy->RotateTowardsAttackerAndRetaliate(GetWeaponOwner());
	engagedEnemy->SetLogicEnabled(true);
	APlayerController* cont = UGameplayStatics::GetPlayerController(this, 0);
	cont->SetViewTargetWithBlend(engagedEnemy, 0.f);
	FTimerHandle changeBackHandle;
	GetWorldTimerManager().SetTimer(changeBackHandle, this, &AFirearm::ReturnToInit, 3.0f);
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


void AFirearm::GunKilledTarget()
{

	if (killedEnemies.Num() > 1)
	{
		for (int i = 0; i < killedEnemies.Num(); i++)
		{
			if (killedEnemies[i] == killedEnemies.Last())
			{
				GetWeaponOwner()->OnUnitDeath(killedEnemies[i]);
				continue;

			}
			killedEnemies[i]->HandleCharacterDeath();
			
		}
	}
	else
	{
		GetWeaponOwner()->OnUnitDeath(killedEnemies[0]);
		
	}
	killedEnemies.Empty();
}
