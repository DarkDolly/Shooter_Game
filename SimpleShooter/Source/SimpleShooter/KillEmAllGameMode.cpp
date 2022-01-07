// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

void AKillEmAllGameMode::PawnKilled(APawn* DeadPawn)
{
	Super::PawnKilled(DeadPawn);

	UE_LOG(LogTemp, Warning, TEXT("A pawn was killed!"));
}