// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh"));
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("You've pulled the trigger"));

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName("MuzzleFlashSocket"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;
	OUT FVector ViewpointLocation;
	OUT FRotator ViewpointRotation;
	OwnerController->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);

	// DrawDebugCamera(GetWorld(), ViewpointLocation, ViewpointRotation, 90, 2, FColor::Cyan, true);

	FVector End = ViewpointLocation + ViewpointRotation.Vector() * MaxRange; // Calculating the end point of our line trace (the end of the vector that comes out of the viewport)
	// TODO LineTrace
	OUT FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, ViewpointLocation, End, ECollisionChannel::ECC_GameTraceChannel1);
	if (bSuccess)
	{
		DrawDebugLine(GetWorld(), ViewpointLocation, End, FColor::Cyan, true, 10);
		// DrawDebugPoint(GetWorld(), End, 20, FColor::Cyan, true, 10);

		FVector ShotDirection = -ViewpointRotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitSplash, Hit.Location, ShotDirection.Rotation());

		AActor* ActorHit = Hit.GetActor();
		if (ActorHit != nullptr)
		{
			FPointDamageEvent DamageEvent(BulletDamage, Hit, ShotDirection, nullptr);
			TakeDamage(BulletDamage, DamageEvent, OwnerController, this);
		}
	}
}
