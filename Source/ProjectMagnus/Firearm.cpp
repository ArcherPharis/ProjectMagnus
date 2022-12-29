// Fill out your copyright notice in the Description page of Project Settings.


#include "Firearm.h"
#include "Kismet/GameplayStatics.h"
#include "Character_Base.h"
#include "BaseEnemy.h"
#include "PRAttributeSet.h"
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

	//if (CurrentAmmo <= MaxAmmo && AmmoReserves != 0 && CurrentAmmo != MaxAmmo)
	//{
	//	int32 Result = MaxAmmo - CurrentAmmo;

	//	if (Result <= AmmoReserves)
	//	{
	//		AmmoReserves -= Result;
	//		CurrentAmmo += Result;
	//	}
	//	else
	//	{
	//		CurrentAmmo += AmmoReserves;
	//		AmmoReserves = 0;
	//	}
	//}
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
				//killedEnemies.Add(damagableChara);
				//GetWorldTimerManager().SetTimer(GunKilledHandle, this, &AFirearm::GunKilledTarget, 1.f, false);
				
			}
			else if (!damagableChara)
			{
				ABaseEnemy* damagableEnemy = Cast<ABaseEnemy>(result.GetActor());
				if (damagableEnemy && damagableEnemy->GetAttributeSet()->GetHealth() == 0)
				{
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

	if (GetCurrentAmmo() == 0)
	{
		//SetInAttackEvent(false);		
		//SetPlayerWantsToStopFiring(false);
		//onEndAttackEvent.Broadcast();
	}
	onForecastInfo.Broadcast(GetBulletsToKill(targetedActor), GetCurrentAmmo());
	onWeaponUse.Broadcast(GetCurrentAmmo(), GetAmmoReserves());
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
	else
	{
		ABaseEnemy* enemy = Cast<ABaseEnemy>(currentTarget);
		float currentTargetHealth = enemy->GetAttributeSet()->GetHealth();
		int shotsToKillE = FMath::CeilToInt(currentTargetHealth / damage);
		shotsToKillE = FMath::Clamp(shotsToKillE, 0, 100);
		return shotsToKillE;
	}
	return 0;
	
}

void AFirearm::GunKilledTarget()
{


	//ACharacter_Base* chr = Cast<ACharacter_Base>(targetedActor);

	//if (chr == nullptr) return;

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



	//if the list of killed units is greater than 1, play a special methd for everything
	//but the last, meaning they will spawn their death actor, but no go through the
	//cam events. They will just die and award exp. The last actor in the list will 
	//go through the OnUnitDeath method. Else, there is only 1 in the list, just carry
	//on normally.
	//make sure to empty the list after this event.
	killedEnemies.Empty();
}
