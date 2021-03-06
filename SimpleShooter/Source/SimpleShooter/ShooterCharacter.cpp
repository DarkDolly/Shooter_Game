// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Components/AudioComponent.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	DeathSound = Cast<UAudioComponent>(GetDefaultSubobjectByName(TEXT("DeathSound")));
	ShotSound = Cast<UAudioComponent>(GetDefaultSubobjectByName(TEXT("ShotSound")));
	
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(FName("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("WeaponSocket"));
	Gun->SetOwner(this);

	//bIsReadyToHeal = (FPlatformTime::Seconds() - LastTakeDamage) > HealDelay; //Healing System
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (bIsReadyToHeal || Health < MaxHealth) //Healing System
	{
		Health = Health + 0.01;
		LastTakeDamage = FPlatformTime::Seconds() + 4;
	}*/
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, /*&AShooterCharacter::LookUp*/ &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	//bool IsReadyToFire = FPlatformTime::Seconds() - LastFireTime > FireDelay;

	//if (IsReadyToFire)
	//{
		Gun->PullTrigger();
	//	LastFireTime = FPlatformTime::Seconds();
	//}
}


float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!IsDead())
	{
		if (Health < 21)
		{
			if (DeathSound != nullptr)
			{
				DeathSound->Play();
			}
		}
		if (Health > 22)
		{
			if (ShotSound != nullptr)
			{
				ShotSound->Play();
			}
		}
		DamageToApply = FMath::Min(Health, DamageToApply);
		Health = Health - DamageToApply;
		UE_LOG(LogTemp, Warning, TEXT("%s health is %f"), *GetActorLabel(), Health);
	}
	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	/*LastTakeDamage = FPlatformTime::Seconds();*/ //Healing System

	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	// if (Health <= 0)
	// {
	// 	return true;
	// }
	// else
	// {
	// 	return false;
	// }
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// void AShooterCharacter::LookUp(float AxisValue)
// {
// 	AddControllerPitchInput(AxisValue);
// }