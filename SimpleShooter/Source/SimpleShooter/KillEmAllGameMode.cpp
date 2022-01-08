// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

void AKillEmAllGameMode::PawnKilled(APawn* DeadPawn)
{
	Super::PawnKilled(DeadPawn);

	APlayerController* PlayerController = Cast<APlayerController>(DeadPawn->GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->GameHasEnded(nullptr, false);
	}
}