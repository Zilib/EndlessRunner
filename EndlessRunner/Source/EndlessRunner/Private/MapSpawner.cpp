// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSpawner.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Obstacle.h"
#include "RunnerCharacter.h"
#include "Engine/StaticMesh.h"

// Sets default values
AMapSpawner::AMapSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMapSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ensure(PreviousCorridor)) { return; }
	if (Corridors.Num() == 0) { return;  } 

	GetWorld()->GetTimerManager().SetTimer(hTimer, this, &AMapSpawner::GenerateMap, .01f, true);
	// That is constant value
	CorridorDisplacement = GetDisplacement(RunnerHero->GetV0Velocity(), TotalFlightTime(), RunnerHero->GetCos()) - DisplacementMarginValue;
}

// Calculate total jump flight time.
float AMapSpawner::TotalFlightTime() const
{
	const float V0 = RunnerHero->GetV0Velocity();
	return  (2 * V0 * RunnerHero->GetSin()) / GetWorld()->GetGravityZ() * -1;
}

// Projectile motion pattern
float AMapSpawner::GetDisplacement(float V0, float t, float Cos0) const
{
	return V0 * t * Cos0;
}

// Separately functions for every corridor
void AMapSpawner::SpawnCorridor()
{
	SpawnedCorridors++;
	PreviousCorridor = GetWorld()->SpawnActor<ACorridor>(
		CorridorToSpawn,
		SpawnPointTransform
		);
}

void AMapSpawner::SpawnStraightCorridor()
{
	CorridorToSpawn = *Corridors.Find(FName("Straight"));
	SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointStraight"));
	// Get position where should be spawned corridor
	if (RandomGenerator(ChanceToGreaterDistance) && SpawnedCorridors >= 2)
	{
		if (RandomGenerator(ChanceToJump))
		{
			// I am looking for max distance where i can reach max jump height
			const float DistanceToReachMaxHeight = (RunnerHero->TimeToReachMaximumHeight() * CorridorDisplacement) / TotalFlightTime();

			FVector SocketLocation = SpawnPointTransform.GetLocation();
			FVector SocketRotationForward = SpawnPointTransform.GetRotation().GetForwardVector() * DistanceToReachMaxHeight;
			FVector SocketRotationUp = SpawnPointTransform.GetRotation().GetUpVector() * (RunnerHero->GetMaxJumpHeight());

			SpawnPointTransform.SetLocation(SocketLocation + SocketRotationForward + SocketRotationUp);
		}
		else
		{
			/// Make distance greater
			FVector SocketLocation = SpawnPointTransform.GetLocation();
			FVector SocketRotationForward = SpawnPointTransform.GetRotation().GetForwardVector() * -(CorridorDisplacement); // TODO find an pattern for perfect distance depend of Vx 
			SpawnPointTransform.SetLocation(SocketLocation + SocketRotationForward);
		}
	}

	SpawnCorridor();
	SpawnObstacleRock();
}

void AMapSpawner::SpawnTurnRightCorridor()
{
	// To avoid a collisions
	CanTurnRight = false;
	CanTurnLeft = true;

	CorridorToSpawn = *Corridors.Find(FName("Turn Right"));

	// Get position where should be spawned corridor
	SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointTurnRight"));

	if (RandomGenerator(ChanceToGreaterDistance) && SpawnedCorridors >= 2)
	{
		if (RandomGenerator(ChanceToJump))
		{
			// I am looking for max distance where i can reach max jump height
			const float DistanceToReachMaxHeight = (RunnerHero->TimeToReachMaximumHeight() * CorridorDisplacement) / TotalFlightTime();

			FVector SocketLocation = SpawnPointTransform.GetLocation();
			FVector SocketRotationForward = SpawnPointTransform.GetRotation().GetRightVector() * -DistanceToReachMaxHeight;
			FVector SocketRotationUp = SpawnPointTransform.GetRotation().GetUpVector() * (RunnerHero->GetMaxJumpHeight());

			SpawnPointTransform.SetLocation(SocketLocation + SocketRotationForward + SocketRotationUp);
		}
		else
		{
			/// Make distance greater
			FVector SocketLocation = SpawnPointTransform.GetLocation();
			FVector SocketRotation = SpawnPointTransform.GetRotation().GetRightVector() * CorridorDisplacement;
			SpawnPointTransform.SetLocation(SocketLocation + SocketRotation);
			//SpawnPointTransform.AddToTranslation(FVector(-DistanceObstacle(), 0, 0));
		}
	}
	SpawnCorridor();
}

void AMapSpawner::SpawnTurnLeftCorridor()
{
	// To avoid a collisions
	CanTurnLeft = false;
	CanTurnRight = true;

	CorridorToSpawn = *Corridors.Find(FName("Turn Left"));

	// Get position where should be spawned corridor
	SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointTurnLeft"));

	if (RandomGenerator(ChanceToGreaterDistance) && SpawnedCorridors >= 2)
	{
		if (RandomGenerator(ChanceToJump))
		{
			// I am looking for max distance where i can reach max jump height
			const float DistanceToReachMaxHeight = (RunnerHero->TimeToReachMaximumHeight() * CorridorDisplacement) / TotalFlightTime();

			FVector SocketLocation = SpawnPointTransform.GetLocation();
			FVector SocketRotationForward = SpawnPointTransform.GetRotation().GetForwardVector() * DistanceToReachMaxHeight;
			FVector SocketRotationUp = SpawnPointTransform.GetRotation().GetUpVector() * (RunnerHero->GetMaxJumpHeight());

			SpawnPointTransform.SetLocation(SocketLocation + SocketRotationForward + SocketRotationUp);
		}
		else
		{
			/// Make distance greater
			FVector SocketLocation = SpawnPointTransform.GetLocation();
			FVector SocketRotation = SpawnPointTransform.GetRotation().GetForwardVector() * -CorridorDisplacement;
			SpawnPointTransform.SetLocation(SocketLocation + SocketRotation);
		}
	}
	SpawnCorridor();
}
////////////////////////

void AMapSpawner::GenerateMap()
{
	// Spawn turn left corridor, let him take a good velocity.
	if (RandomGenerator(ChanceToTurnLeft)
		&& (CanTurnLeft == true))
	{
		SpawnTurnLeftCorridor();
	}
	else if (RandomGenerator(ChanceToTurnRight)
		&& (CanTurnRight == true))
	{
		SpawnTurnRightCorridor();
	}
	else
	{
		SpawnStraightCorridor();
	}
}

// Random system, to make some obstacles.
bool AMapSpawner::RandomGenerator(int Chance)
{
	int32 HitNumber = FMath::FRandRange(1, 100);
	return HitNumber <= Chance;
}

// Called every frame
void AMapSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Is spawned enought, player will not see spawning system, now let's protect his memory
	if (SpawnedCorridors >= StartToSpawnCorridor && !NextDelayTimeActive)
	{
		NextDelayTimeActive = true; // Spawned corridors can be faster than CPU, so that is for protect
		GetWorld()->GetTimerManager().ClearTimer(hTimer);
		GetWorld()->GetTimerManager().SetTimer(hTimer, this, &AMapSpawner::GenerateMap, NextDelay, true);
	}
}

// Only work with straight corridor
void AMapSpawner::SpawnObstacleRock()
{
	int32 SpawnPointNumber = FMath::FRandRange(1, 99);
	FTransform RockSpawnTransform;
	// Spawn at left point
	if (SpawnPointNumber%2 == 0 && SpawnPointNumber%3 != 0 && SpawnPointNumber % 5 != 0)
	{
		RockSpawnTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnObstacleLeft"));
	}
	else if (SpawnPointNumber%3 == 2 && SpawnPointNumber%2 != 0 && SpawnPointNumber % 5 != 0)
	{
		 RockSpawnTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnObstacleCenter"));
	} // Spawn at center
	else
	{
		RockSpawnTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnObstacleRight"));
	} // Spawn at right

	PreviousCorridor->SpawnedObstacleRock = GetWorld()->SpawnActor<AObstacle>(
		ObstacleBP,
		RockSpawnTransform
	);
}
