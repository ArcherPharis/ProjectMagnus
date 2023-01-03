// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"
#include "Weapon.h"
#include "Firearm.h"
#include "StatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PMGameModeBase.h"
#include "PRAbilitySystemComponent.h"
#include "PRGameplayAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PRAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalGear.h"
#include "SupportGear.h"


// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	abilitySystemComp = CreateDefaultSubobject<UPRAbilitySystemComponent>(TEXT("Ability System Comp"));
	attributeSet = CreateDefaultSubobject<UPRAttributeSet>(TEXT("Attribute Set"));

}



void ACharacter_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	abilitySystemComp->InitAbilityActorInfo(this, this);}

void ACharacter_Base::SetIsAiming(bool value)
{
	bIsAiming = value;
}

void ACharacter_Base::ToggleInput(bool enableInput)
{

	if (enableInput)
	{
		APlayerController* pc = Cast<APlayerController>(GetController());
		pc->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		APlayerController* pc = Cast<APlayerController>(GetController());
		pc->SetInputMode(FInputModeUIOnly());
	}
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	gameMode = Cast<APMGameModeBase>(UGameplayStatics::GetGameMode(this));
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &ACharacter_Base::CharacterDied);
	ApplyInitialEffect();
	GiveAbility(SprintAbility);
	for (auto& abilityKeyValuePair : InitialAbilities)
	{
		GiveAbility(abilityKeyValuePair.Value, static_cast<int>(abilityKeyValuePair.Key), true);
	}
	
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrainStamina();

}

void ACharacter_Base::SetKiller(AActor* killer)
{
	Killer = killer;
	APMGameModeBase* gm = Cast<APMGameModeBase>(UGameplayStatics::GetGameMode(this));
	gm->AddDownedUnits(this);


}

void ACharacter_Base::HandleCharacterDeath()
{
	SetActorHiddenInGame(true);
	GetCurrentWeapon()->SetActorHiddenInGame(true);
	APawn* DeathPawn = GetWorld()->SpawnActor<APawn>(GetDeathPawnClass(),GetTransform());

}

void ACharacter_Base::PlayGunAttackClip()
{
	if(GunAttackClip)
	UGameplayStatics::PlaySoundAtLocation(this, GunAttackClip, GetActorLocation(), 1.f);
}

// Called to bind functionality to input
void ACharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void ACharacter_Base::OnUnitDeath(ACharacter_Base* characterToDie)
{

	
	GetWorldTimerManager().SetTimer(returnToUnitHandle, this, &ACharacter_Base::AfterUnitDeath, 6.5f, false);
	//inGameUI->HideHUD();
	characterToDie->SetActorHiddenInGame(true);
	APawn* DeathPawn = GetWorld()->SpawnActor<APawn>(characterToDie->GetDeathPawnClass(), characterToDie->GetTransform());
	APlayerController* cont = Cast<APlayerController>(GetController());
	characterToDie->GetCurrentWeapon()->SetActorHiddenInGame(true);
	cont->SetViewTargetWithBlend(DeathPawn, 0.5f);
	

}

float ACharacter_Base::GetCurrentWeight()
{
	return equippedWeapon->GetWeight();
}

void ACharacter_Base::GunAttackEventEnd()
{
	PostFireEventEffects();
}

void ACharacter_Base::ChangeAP(int amount)
{
	GetAttributeSet()->SetActionPoints(GetAttributeSet()->GetActionPoints() + amount);
}

bool ACharacter_Base::CharacterCanAct()
{
	if (GetAttributeSet()->GetActionPoints() > 0)
	{
		return true;
	}
	return false;
}

void ACharacter_Base::SetSprinting(bool value)
{
	isSprinting = value;
}

UAbilitySystemComponent* ACharacter_Base::GetAbilitySystemComponent() const
{
	return abilitySystemComp;
}

void ACharacter_Base::GetArmorValue(float& armor, float& maxArmor)
{
	armor = GetAttributeSet()->GetArmor();
	maxArmor = GetAttributeSet()->GetMaxArmor();
}

void ACharacter_Base::AfterUnitDeath()
{
	APlayerController* cont = UGameplayStatics::GetPlayerController(this, 0);
	cont->SetViewTargetWithBlend(this, 0.5f);
	cont->SetInputMode(FInputModeGameOnly());
	GetCurrentWeapon()->SetInAttackEvent(false);
	StopAiming();
	GunAttackEventEnd();

}

void ACharacter_Base::GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID, bool broadCast)
{
	FGameplayAbilitySpecHandle AbilitySpecHandle = abilitySystemComp->GiveAbility(FGameplayAbilitySpec(newAbility, 1, inputID));

	if (broadCast)
	{
		onAbilityAdded.Broadcast(Cast<UPRGameplayAbilityBase>(GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability));
	}
}

void ACharacter_Base::DrainStamina()
{
	if (isSprinting) return;
	if (GetVelocity().Length() > 0 && !isMoving)
	{
		isMoving = true;
		FGameplayEffectContextHandle handle;
		GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(staminaDrainEffect.GetDefaultObject(), -1, handle);
	}
	else if (GetVelocity().Length() == 0)
	{

		if (isMoving == false) return;
		OnStopMoving();
		isMoving = false;
	}
}

void ACharacter_Base::CharacterDied(const FOnAttributeChangeData& AttributeData)
{
	

	if (AttributeData.NewValue == 0)
	{
		//GetMesh()->GetAnimInstance()->Montage_Play(onDeadMontage);
		isDead = true;
		
		//SetActorEnableCollision(false);
	}
}

void ACharacter_Base::Attack()
{
	equippedWeapon->bFireButtonPressed = true;


	if (equippedWeapon->GetInAttackEvent())
	{
		UE_LOG(LogTemp, Warning, TEXT("In attack event"));
		return;
	}

	if (bIsAiming)
	{
		equippedWeapon->Attack();
	}
}

void ACharacter_Base::StopAttack()
{
	equippedWeapon->bFireButtonPressed = false;

}

void ACharacter_Base::StopFiring()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to stop firing"));
	GetCurrentWeapon()->SetPlayerWantsToStopFiring(true);
}

void ACharacter_Base::LevelUp()
{
}


void ACharacter_Base::ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply)
{
	FGameplayEffectSpecHandle InitialSpec = abilitySystemComp->MakeOutgoingSpec(effectToApply, -1, abilitySystemComp->MakeEffectContext());
	abilitySystemComp->ApplyGameplayEffectSpecToSelf(*InitialSpec.Data);
}

void ACharacter_Base::ApplyInitialEffect()
{
	for (auto effect : InitialEffects)
	{
		ApplyEffectToSelf(effect);
	}
}

bool ACharacter_Base::IsCharacterOutOfStamina()
{
	if (GetAttributeSet()->GetStamina() == 0)
	{
		return true;
	}

	return false;
}

void ACharacter_Base::UseItem()
{
	
}

void ACharacter_Base::GiveEquipment()
{
	if (weaponClass)
	{
		AWeapon* weapon;
		weapon = GetWorld()->SpawnActor<AWeapon>(weaponClass);
		weapon->SetWeaponOwner(this);
		weapon->OnEquip(GetMesh());
		equippedWeapon = weapon;
		onWeaponEquipped.Broadcast(equippedWeapon);

	}

	if (tacticalGearClass)
	{
		ATacticalGear* tGear;
		tGear = GetWorld()->SpawnActor<ATacticalGear>(tacticalGearClass);
		tGear->SetOwner(this);
		tGear->AttachToCharacterMesh(GetMesh());
		CurrentlyEquippedTacticalGear = tGear;
	}

	if (supportGearClass)
	{
		ASupportGear* sGear;
		sGear = GetWorld()->SpawnActor<ASupportGear>(supportGearClass);
		sGear->SetOwner(this);
		sGear->AttachToCharacterMesh(GetMesh());
		CurrentlyEquippedSupportGear = sGear;
	}

}


void ACharacter_Base::Aim()
{
	if (!bIsAiming)
	{
		bIsAiming = true;
		GetCharacterMovement()->MaxWalkSpeed = aimSpeedValue;
		AFirearm* fireArm = Cast<AFirearm>(GetCurrentWeapon());
		fireArm->FirearmAim();
	}

	
}

void ACharacter_Base::StopAiming()
{

	
	bIsAiming = false;
	GetCharacterMovement()->MaxWalkSpeed = originalSpeedValue;
	StopAttack();
	AFirearm* fireArm = Cast<AFirearm>(GetCurrentWeapon());
	if (fireArm)
	{
		fireArm->StopFirearmAim();
	}

	
}

void ACharacter_Base::Sprint()
{
	if (!bIsAiming)
	abilitySystemComp->TryActivateAbilityByClass(SprintAbility);
	GetCharacterMovement()->MaxWalkSpeed = sprintValue;
}

void ACharacter_Base::StopSprint()
{
	if (!bIsAiming)
	onStoppedSprinting.Broadcast();
	GetCharacterMovement()->MaxWalkSpeed = originalSpeedValue;

}

