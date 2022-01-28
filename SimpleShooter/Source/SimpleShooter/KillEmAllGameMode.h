// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AKillEmAllGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PawnKilled(APawn* DeadPawn) override;

	UFUNCTION(BlueprintPure)
	int32 GetEnemiesLeft();

protected:

private:
	void EndGame(bool bIsPlayerWinner);

	UPROPERTY(EditAnywhere)
	int32 Enemies = 10;

};
