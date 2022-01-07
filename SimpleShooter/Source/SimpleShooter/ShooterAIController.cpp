// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn != nullptr && LineOfSightTo(PlayerPawn))
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation")));
		
		// MoveToActor(PlayerPawn, AcceptanceRadius);
		SetFocus(PlayerPawn, EAIFocusPriority::Gameplay);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
		
		ClearFocus(EAIFocusPriority::Gameplay);
	}
}