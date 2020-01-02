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
	if (Corridors.Num() == 0) { return; }

	GetWorld()->GetTimerManager().SetTimer(hTimer, this, &AMapSpawner::GenerateMap, .01f, true);

	GameInstance = Cast<UMyGameInstance>(GetGameInstance());

	GameInstance->ItemValue = this->ItemValue;
	GameInstance->Acceleration = this->Acceleration;
	GameInstance->MaxPlayerSpeed = this->MaxPlayerSpeed;
}

// Calculate total jump flight time.
float AMapSpawner::TotalFlightTime() const
{
	const float V0 = RunnerHero->GetV0Velocity();
	return  (2 * V0 * RunnerHero->GetSin()) / GetWorld()->GetGravityZ() * -1;
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
	if (RandomGenerator(ChanceToGreaterDistance) 
		&& SpawnedCorridors >= 2
		&& LastWasTurn == false // Do not displacement next object, because player don't have time to embrace it.
		)
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
	LastWasTurn = false;

	SpawnCorridor();
	SpawnItem();
	if (SpawnedCorridors > WaitCorridors) // Wait after start game "Wait Corridors" corridors before start spawning rocks
	{
		SpawnObstacleRock();
	}
}

void AMapSpawner::SpawnTurnRightCorridor()
{
	// To avoid a collisions
	CanTurnRight = false;
	CanTurnLeft = true;

	CorridorToSpawn = *Corridors.Find(FName("Turn Right"));

	// Get position where should be spawned corridor
	SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointTurnRight"));

	if (RandomGenerator(ChanceToGreaterDistance) 
		&& SpawnedCorridors >= 2
		&& LastWasTurn == false // Do not displacement next object, because player don't have time to embrace it.
		)
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
	LastWasTurn = true;
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

	if (RandomGenerator(ChanceToGreaterDistance)
		&& SpawnedCorridors >= 2
		&& LastWasTurn == false // Do not displacement next object, because player don't have time to embrace it.
		)
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
	LastWasTurn = true;
}

////////////////////////

void AMapSpawner::GenerateMap()
{
	if (RunnerHero->GetVelocity() == FVector(0,0,0)) { return; } // Spawn whenever player is moving. Don't spawn to much objects when it is not necessary

	// Player Speed is Vx, and RunnerJumpZVelocity is Vy it is equal to V0
	UE_LOG(LogTemp, Warning, TEXT("%f"), GameInstance->PlayerSpeed);
	CorridorDisplacement = GetDisplacement(
		GameInstance->PlayerSpeed,
		RunnerHero->GetJumpZVelocity(),
		TotalFlightTime(), RunnerHero->GetCos()) - DisplacementMarginValue;

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

void AMapSpawner::SpawnItem()
{
	int32 SocketNumber = FMath::FRandRange(1,3); // Item can be only spawned in 3 places, so make random choice.
	FTransform NextItemSpawnTransform; // Here will be next spawn transform

	// Every item can be spawned only at the entry of straight corridor.
	if (SocketNumber == 1)
	{
		NextItemSpawnTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointItem1")); // Spawn at left side
	}
	else if (SocketNumber == 2)
	{
		NextItemSpawnTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointItem2")); // Spawn at center
	}
	else if (SocketNumber == 3)
	{
		NextItemSpawnTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointItem3")); // Spawn at right side
	}
	PreviousCorridor->SpawnedItem = GetWorld()->SpawnActor<ACoin>(
		ItemToSpawn,
		NextItemSpawnTransform
	);
}
