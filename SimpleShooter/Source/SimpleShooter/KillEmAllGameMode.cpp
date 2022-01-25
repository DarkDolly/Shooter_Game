// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* DeadPawn)
{
	Super::PawnKilled(DeadPawn);

	APlayerController* PlayerController = Cast<APlayerController>(DeadPawn->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
		/*PlayerController->GameHasEnded(nullptr, false);*/ // How I ended the game before the EndGame() function existed
	}

	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!AIController->IsDead())
		{
			return;
		}
	}
	
	EndGame(true);

	// for loop over ShooterAI in World:
		// Is not dead?
			// return

	// End game
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn() /*This sets the camera (focus) on something, in this case the controller's own pawn but could be the winner pawn etc*/, bIsWinner);
		
		/*bool bIsPlayerController = Controller->IsPlayerController(); // Another way of doing what's above
		if (bIsPlayerWinner)
		{
			Controller->GameHasEnded(nullptr, bIsPlayerController);
		}
		else
		{
			Controller->GameHasEnded(nullptr, !bIsPlayerController);
		}*/
	}
}