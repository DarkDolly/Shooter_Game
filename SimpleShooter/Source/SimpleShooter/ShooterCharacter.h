// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;
class UAudioComponent;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void Shoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	//float FireDelay = 1.0f;
	//double LastFireTime = 0;

	// void LookUp(float AxisValue);

	UPROPERTY(EditAnywhere)
	float RotationRate = 70.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UAudioComponent* DeathSound = nullptr;
	UAudioComponent* ShotSound = nullptr;
};
