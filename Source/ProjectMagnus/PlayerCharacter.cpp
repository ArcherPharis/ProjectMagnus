// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StatComponent.h"

APlayerCharacter::APlayerCharacter()
{

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(RootComponent);
	playerEye = CreateDefaultSubobject<UCameraComponent>(TEXT("playerEye"));
	playerEye->SetupAttachment(springArm);
	bUseControllerRotationYaw = false;
	

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
}

void APlayerCharacter::MoveForward(float value)
{

	if (!GetStatComponent()) return;
	if(GetStatComponent()->GetCurrentStamina() > 0)
	AddMovementInput(FRotationMatrix(GetControlRotator()).GetScaledAxis(EAxis::X) * value);
	if (value != 0 && !bHasAlreadyStartedMoving)
	{
		bHasAlreadyStartedMoving = true;

		if (value < 0)
		{
			//todo, refactor soonish, so that enemies also drain stamina
			//this method is ok, but does not change drain when moving slower.
			//so, we should instead drain this, by velocity of actor instead.
			GetStatComponent()->DrainStamina(-value);
		}
		else
		{
			GetStatComponent()->DrainStamina(value);
		}
	}
	else
	{
		bHasAlreadyStartedMoving = false;

	}
}

void APlayerCharacter::MoveRight(float value)
{
	if (!GetStatComponent()) return;
	if (GetStatComponent()->GetCurrentStamina() > 0)
	AddMovementInput(FRotationMatrix(GetControlRotator()).GetScaledAxis(EAxis::Y) * value);
	if (value != 0 && !bHasAlreadyStartedMoving)
	{
		bHasAlreadyStartedMoving = true;
		if (value < 0)
		{
			GetStatComponent()->DrainStamina(-value);
		}
		else
		{
			GetStatComponent()->DrainStamina(value);
		}
	}
	else
	{
		bHasAlreadyStartedMoving = false;
	}
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

void APlayerCharacter::Attack()
{
	Super::Attack();
}

void APlayerCharacter::StopAttack()
{
	Super::StopAttack();
}

