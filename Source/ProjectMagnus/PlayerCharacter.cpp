// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StatComponent.h"
#include "AbilitySystemComponent.h"
#include "PRAttributeSet.h"
#include "Firearm.h"

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
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACharacter_Base::Sprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACharacter_Base::StopSprint);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &APlayerCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &APlayerCharacter::StopAiming);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this, &APlayerCharacter::StopAttack);
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
	onUpdateHealthStamRange.Broadcast(GetAttributeSet()->GetMaxHealth(), GetAttributeSet()->GetMaxStamina());
}

void APlayerCharacter::MoveForward(float value)
{

	if (IsCharacterOutOfStamina())
	{
		StopSprint();
		return;
	}
	AddMovementInput(FRotationMatrix(GetControlRotator()).GetScaledAxis(EAxis::X) * value);
	
}

void APlayerCharacter::MoveRight(float value)
{

	if (IsCharacterOutOfStamina())
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
	

}

void APlayerCharacter::StopAiming()
{
	
	Super::StopAiming();
	playerEye->SetFieldOfView(nonAimFOV);
}

void APlayerCharacter::OnUnitDeath(ACharacter_Base* characterToDie)
{
	DeathEvent();
	Super::OnUnitDeath(characterToDie);
}

void APlayerCharacter::DisplayTargetInfo()
{


	FHitResult result;
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector CameraEnd = cameraManager->GetCameraLocation() + cameraManager->GetActorForwardVector() * 5000;
	FCollisionShape cyl = FCollisionShape::MakeSphere(100);
	if (GetWorld()->LineTraceSingleByChannel(result, cameraManager->GetCameraLocation(), CameraEnd, ECC_GameTraceChannel1))
	{
		onDisplayTargetInfo.Broadcast(true);
		ACharacter_Base* target = Cast<ACharacter_Base>(result.GetActor());
		onUnitTarget.Broadcast(target, target->GetAttributeSet()->GetHealth(), target->GetAttributeSet()->GetMaxHealth());
	}
	else
	{
		onDisplayTargetInfo.Broadcast(false);
	}


}

void APlayerCharacter::Attack()
{
	Super::Attack();
}

void APlayerCharacter::StopAttack()
{
	Super::StopAttack();
}

