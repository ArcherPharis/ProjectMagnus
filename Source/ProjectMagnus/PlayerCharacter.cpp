// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StatComponent.h"
#include "PMGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "PRAttributeSet.h"
#include "Firearm.h"
#include "BaseEnemy.h"

APlayerCharacter::APlayerCharacter()
{

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(RootComponent);
	playerEye = CreateDefaultSubobject<UCameraComponent>(TEXT("playerEye"));
	playerEye->SetupAttachment(springArm);
	bUseControllerRotationYaw = false;
	

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DisplayTargetInfo();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::LookRight);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACharacter_Base::Interact);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACharacter_Base::Sprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACharacter_Base::StopSprint);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &APlayerCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &APlayerCharacter::StopAiming);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayerAttack);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this, &APlayerCharacter::StopAttack);
	PlayerInputComponent->BindAction("OpenUnitMenu", EInputEvent::IE_Pressed, this, &APlayerCharacter::OpenUnitMenu);
	GetAbilitySystemComponent()->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("Confirm",
		"Cancel",
		"EPRAbilityInputID",
		static_cast<int32>(EPRAbilityInputID::Confirm),
		static_cast<int32>(EPRAbilityInputID::Cancel)));



}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(this, 0)->PlayerCameraManager->ViewPitchMin = -70.f;
	UGameplayStatics::GetPlayerController(this, 0)->PlayerCameraManager->ViewPitchMax = 60.f;
	onUnitGiven.Broadcast(this);
	
	
	
}



void APlayerCharacter::OnDeployed()
{
	onWeaponEquipped.Broadcast(GetCurrentWeapon());
	onAPGauge.Broadcast(GetAttributeSet()->GetActionPoints());
	onUpdateHealthStamRange.Broadcast(GetAttributeSet()->GetMaxHealth(), GetAttributeSet()->GetMaxStamina(), GetAttributeSet()->GetExperiencePoints(), GetAttributeSet()->GetMaxExperiencePoints(), GetAttributeSet()->GetHealth(), GetAttributeSet()->GetStamina());
	ToggleUseControlRotationYaw(false);
	GetGameMode()->SetCurrentDeployedPlayerUnit(this);
	GetGameMode()->ToggleEnemyLogic(true);
	GetGameMode()->TogglePlayerLogic(false);
	UseSkillsOnDeploy();

}

void APlayerCharacter::LevelUp()
{
	float v, s, str, e, a, d;
	GetAttributeSet()->SetLevel(GetAttributeSet()->GetLevel() + 1);
	GenerateHealthLevelUp(v);
	GenerateStaminaLevelUp(s);
	GenerateStrengthLevelUp(str);
	GenerateEnduranceLevelUp(e);
	GenerateAgilityLevelUp(a);
	GenerateDexterityLevelUp(d);
	onInitiateLevelUp.Broadcast(this, v, s, str, e, a, d);
}

void APlayerCharacter::MoveForward(float value)
{

	if (IsCharacterOutOfStamina() || GetIsAiming() || GetIsDead() || GetIsUsingGear())
	{
		StopSprint();
		return;
	}
	AddMovementInput(FRotationMatrix(GetControlRotator()).GetScaledAxis(EAxis::X) * value);
	
}

void APlayerCharacter::MoveRight(float value)
{

	if (IsCharacterOutOfStamina() || GetIsAiming() || GetIsDead() || GetIsUsingGear())
	{
		StopSprint();
		return;
	}
	AddMovementInput(FRotationMatrix(GetControlRotator()).GetScaledAxis(EAxis::Y) * value);

}

void APlayerCharacter::LookRight(float value)
{

	AddControllerYawInput(value);
}

void APlayerCharacter::LookUp(float value)
{

	AddControllerPitchInput(value);
}

FRotator APlayerCharacter::GetControlRotator()
{
	float controlRotationYaw = this->GetControlRotation().Yaw;
	FRotator controlRotator(0.0f, controlRotationYaw, 0.0f);
	return controlRotator;
	
}

void APlayerCharacter::Aim()
{

	Super::Aim();
	playerEye->SetFieldOfView(aimFOV);
	GetGameMode()->ToggleEnemyLogic(false);
	GetGameMode()->TogglePlayerLogic(false);
	SetIsAiming(true);
	

}

void APlayerCharacter::OpenUnitMenu()
{
	onShowUnitMenu.Broadcast();
	APlayerController* cont = UGameplayStatics::GetPlayerController(this, 0);
	cont->SetInputMode(FInputModeUIOnly());
	cont->bShowMouseCursor = true;
	UGameplayStatics::SetGamePaused(this, true);
}

void APlayerCharacter::StopAiming()
{
	
	Super::StopAiming();
	playerEye->SetFieldOfView(nonAimFOV);
	SetIsAiming(false);
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &APlayerCharacter::ReenableAILogic, 0.3f);

	
}

void APlayerCharacter::OnUnitDeath(ACharacter_Base* characterToDie)
{
	
	Super::OnUnitDeath(characterToDie);
}

void APlayerCharacter::SetClassName(FName name)
{
	unitClassName = name;
}

void APlayerCharacter::CharacterDied(const FOnAttributeChangeData& AttributeData)
{
	Super::CharacterDied(AttributeData);
}

void APlayerCharacter::ReenableAILogic()
{
	GetGameMode()->ToggleEnemyLogic(true);
	GetGameMode()->TogglePlayerLogic(true);
}

void APlayerCharacter::DisplayTargetInfo()
{


	FHitResult result;
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector CameraEnd = cameraManager->GetCameraLocation() + cameraManager->GetActorForwardVector() * 5000;
	FCollisionShape cyl = FCollisionShape::MakeSphere(100);
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(result, cameraManager->GetCameraLocation(), CameraEnd, ECC_GameTraceChannel2, CollisionParameters))
	{
		onDisplayTargetInfo.Broadcast(true);
		ACharacter_Base* target = Cast<ACharacter_Base>(result.GetActor());
		
		if (target)
		{
			onUnitTarget.Broadcast(target, target->GetAttributeSet()->GetHealth(), target->GetAttributeSet()->GetMaxHealth(), target->GetAttributeSet()->GetArmor(), target->GetAttributeSet()->GetMaxArmor());
		}
		else
		{
			ABaseEnemy* enemy = Cast<ABaseEnemy>(result.GetActor());
			int toKill, toBreak;
			GetCurrentWeapon()->GetDamageInfo(enemy, toBreak, toKill);
			onEnemyUnitTarget.Broadcast(enemy, enemy->GetAttributeSet()->GetHealth(), enemy->GetAttributeSet()->GetMaxHealth(), enemy->GetAttributeSet()->GetArmor(), enemy->GetAttributeSet()->GetMaxArmor(), toKill, toBreak);
		}

		
	}
	else
	{
		onDisplayTargetInfo.Broadcast(false);
	}


}

void APlayerCharacter::GenerateHealthLevelUp(float& oldHealth)
{
	int roll = FMath::RandRange(1, 100);
	oldHealth = GetAttributeSet()->GetMaxHealth();

	if (roll <= healthGrowthRate)
	{
		
		GetAttributeSet()->SetMaxHealth(GetAttributeSet()->GetMaxHealth() + healthMaxRoll);
	}
	else
	{
		
		GetAttributeSet()->SetMaxHealth(GetAttributeSet()->GetMaxHealth() + healthMinRoll);
	}


}

void APlayerCharacter::GenerateStaminaLevelUp(float& oldS)
{
	int roll = FMath::RandRange(1, 100);
	oldS = GetAttributeSet()->GetMaxStamina();

	if (roll <= staminaGrowthRate)
	{

		GetAttributeSet()->SetMaxStamina(GetAttributeSet()->GetMaxStamina() + staminaMaxRoll);
	}
	else
	{

		GetAttributeSet()->SetMaxStamina(GetAttributeSet()->GetMaxStamina() + staminaMinRoll);

	}
}

void APlayerCharacter::GenerateStrengthLevelUp(float& oldS)
{
	int roll = FMath::RandRange(1, 100);
	oldS = GetAttributeSet()->GetStrength();

	if (roll <= strengthGrowthRate)
	{

		GetAttributeSet()->SetStrength(GetAttributeSet()->GetStrength() + strengthMaxRoll);
	}
	else
	{

		GetAttributeSet()->SetStrength(GetAttributeSet()->GetStrength() + strengthMinRoll);

	}
}

void APlayerCharacter::GenerateEnduranceLevelUp(float& oldE)
{
	int roll = FMath::RandRange(1, 100);
	oldE = GetAttributeSet()->GetEndurance();

	if (roll <= enduranceGrowthRate)
	{

		GetAttributeSet()->SetEndurance(GetAttributeSet()->GetEndurance() + enduranceMaxRoll);
	}
	else
	{

		GetAttributeSet()->SetEndurance(GetAttributeSet()->GetEndurance() + enduranceMinRoll);

	}
}

void APlayerCharacter::GenerateAgilityLevelUp(float& oldA)
{
	int roll = FMath::RandRange(1, 100);
	oldA = GetAttributeSet()->GetAgility();

	if (roll <= agilityGrowthRate)
	{

		GetAttributeSet()->SetAgility(GetAttributeSet()->GetAgility() + agilityMaxRoll);
	}
	else
	{

		GetAttributeSet()->SetAgility(GetAttributeSet()->GetAgility() + agilityMinRoll);

	}
}

void APlayerCharacter::GenerateDexterityLevelUp(float& oldD)
{
	int roll = FMath::RandRange(1, 100);
	oldD = GetAttributeSet()->GetDexterity();

	if (roll <= dexterityGrowthRate)
	{

		GetAttributeSet()->SetDexterity(GetAttributeSet()->GetDexterity() + dexterityMaxRoll);
	}
	else
	{

		GetAttributeSet()->SetDexterity(GetAttributeSet()->GetDexterity() + dexterityMinRoll);

	}
}

void APlayerCharacter::PlayerAttack()
{
	Super::PlayerAttack();
}

void APlayerCharacter::StopAttack()
{
	Super::StopAttack();
}

