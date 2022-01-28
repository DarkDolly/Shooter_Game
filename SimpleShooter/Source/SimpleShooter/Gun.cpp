// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/ShapeComponent.h"

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
//void AGun::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AGun::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("You've pulled the trigger"));

	bool IsReadyToFire = FPlatformTime::Seconds() - LastFireTime > FireDelay;

	if (IsReadyToFire)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, FName("MuzzleFlashSocket"));

		OUT FHitResult Hit;
		OUT FVector ShotDirection;
		bool bSuccess = GunTrace(Hit, ShotDirection);
		if (bSuccess)
		{
			// DrawDebugLine(GetWorld(), ViewpointLocation, End, FColor::Cyan, true, 10);
			// DrawDebugPoint(GetWorld(), End, 20, FColor::Cyan, true, 10);

			AActor* ActorHit = Hit.GetActor();

			if (ActorHit->FindComponentByClass<UShapeComponent>())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitSplashPawn, Hit.Location, ShotDirection.Rotation());
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundPawn, Hit.Location);
			}
			if (!ActorHit->FindComponentByClass<UShapeComponent>())
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitSplash, Hit.Location, ShotDirection.Rotation());
			}
			if (ActorHit != nullptr)
			{
			FPointDamageEvent DamageEvent(BulletDamage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			ActorHit->TakeDamage(BulletDamage, DamageEvent, OwnerController, this);
			}
		}
		LastFireTime = FPlatformTime::Seconds();
	}
}


bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	OUT FVector ViewpointLocation;
	OUT FRotator ViewpointRotation;
	OwnerController->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);
	ShotDirection = -ViewpointRotation.Vector();

	//DrawDebugCamera(GetWorld(), ViewpointLocation, ViewpointRotation, 90, 2, FColor::Cyan, true);

	FVector End = ViewpointLocation + ViewpointRotation.Vector() * MaxRange; // Calculating the end point of our line trace (the end of the vector that comes out of the viewport)

	OUT FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, /*ViewportLocation //Changed this because I want the shot to come from a socket in the gun and not from the viewport*/Mesh->GetSocketLocation(FName("BulletSpawnPoint")), End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}