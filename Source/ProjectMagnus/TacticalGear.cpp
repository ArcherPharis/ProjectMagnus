// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalGear.h"
#include "Projectile.h"

void ATacticalGear::ThrowGear(float power, FVector direction)
{
	AProjectile* projectileSpawned =  GetWorld()->SpawnActor<AProjectile>(projectileClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	projectileSpawned->SetProjectileSpeed(power , direction);
	projectileSpawned->SetOwner(this);
	projectileSpawned->SetEventTag(TacticalDamageEventTag);
	
	UseGear();

}

void ATacticalGear::UseGear()
{
	Super::UseGear();
}
