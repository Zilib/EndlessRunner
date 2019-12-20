// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSpawner.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
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

	GetWorld()->GetTimerManager().SetTimer(hTimer, this, &AMapSpawner::GenerateMap, .5f, true);
}

// Calculate total jump flight time.
float AMapSpawner::TotalFlightTime() const
{
	const float V0 = RunnerHero->GetV0Velocity();
	return  (2 * V0 * RunnerHero->GetSin()) / GetWorld()->GetGravityZ() * -1;
}

// Calculate difference in X forward vector
float AMapSpawner::DistanceObstacle() const
{
	if (!ensure(RunnerHero)) { return 0; }

	const float Td = TotalFlightTime();
	const float V0 = RunnerHero->GetV0Velocity();

	// Calculate from the horizontal displacement the maximum distance of projectile
	float d = V0 * Td * RunnerHero->GetCos();

	return d;
}

void AMapSpawner::GenerateMap()
{
	FTransform SpawnPointTransform;
	TSubclassOf<ACorridor> CorridorToSpawn = NULL;
	// Spawn turn left corridor
	if (RandomGenerator(ChanceToTurnLeft) && CanTurnLeft == true)
	{
		// To avoid a collisions
		CanTurnLeft = false;
		CanTurnRight = true;

		CorridorToSpawn = *Corridors.Find(FName("Turn Left"));

		// Get position where should be spawned corridor
		SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointTurnLeft"));

		if (RandomGenerator(ChanceToGreaterDistance))
		{
			if (RandomGenerator(ChanceToJump))
			{
				// I am looking for max distance where i can reach max jump height
				const float DistanceToReachMaxHeight = (RunnerHero->TimeToReachMaximumHeight() * DistanceObstacle()) / TotalFlightTime();

				FVector SocketLocation = SpawnPointTransform.GetLocation();
				FVector SocketRotationForward = SpawnPointTransform.GetRotation().GetForwardVector() * DistanceToReachMaxHeight;
				FVector SocketRotationUp = SpawnPointTransform.GetRotation().GetUpVector() * (RunnerHero->GetMaxJumpHeight() + 30);

				SpawnPointTransform.SetLocation(SocketLocation + SocketRotationForward + SocketRotationUp);
			}
			else
			{
				/// Make distance greater
				FVector SocketLocation = SpawnPointTransform.GetLocation();
				FVector SocketRotation = SpawnPointTransform.GetRotation().GetForwardVector() * -DistanceObstacle();
				SpawnPointTransform.SetLocation(SocketLocation + SocketRotation);
			}
		}
	}
	else if (RandomGenerator(ChanceToTurnRight) && CanTurnRight == true)
	{
		// To avoid a collisions
		CanTurnRight = false;
		CanTurnLeft = true;

		CorridorToSpawn = *Corridors.Find(FName("Turn Right"));

		// Get position where should be spawned corridor
		SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointTurnRight"));

		if (RandomGenerator(ChanceToGreaterDistance))
		{
			if (RandomGenerator(ChanceToJump))
			{
				// I am looking for max distance where i can reach max jump height
				const float DistanceToReachMaxHeight = (RunnerHero->TimeToReachMaximumHeight() * DistanceObstacle()) / TotalFlightTime();

				FVector SocketLocation = SpawnPointTransform.GetLocation();
				FVector SocketRotationForward = SpawnPointTransform.GetRotation().GetRightVector() * -DistanceToReachMaxHeight;
				FVector SocketRotationUp = SpawnPointTransform.GetRotation().GetUpVector() * (RunnerHero->GetMaxJumpHeight() + 30);

				SpawnPointTransform.SetLocation(SocketLocation + SocketRotationForward + SocketRotationUp);
			}
			else
			{
				/// Make distance greater
				FVector SocketLocation = SpawnPointTransform.GetLocation();
				FVector SocketRotation = SpawnPointTransform.GetRotation().GetRightVector() * DistanceObstacle();
				SpawnPointTransform.SetLocation(SocketLocation + SocketRotation);
				//SpawnPointTransform.AddToTranslation(FVector(-DistanceObstacle(), 0, 0));
			}
		}
	}
	else
	{
		CorridorToSpawn = *Corridors.Find(FName("Straight"));
		SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPointStraight"));

		// Get position where should be spawned corridor
		if (RandomGenerator(ChanceToGreaterDistance))
		{
			if (RandomGenerator(ChanceToJump))
			{
				// I am looking for max distance where i can reach max jump height
				const float DistanceToReachMaxHeight = (RunnerHero->TimeToReachMaximumHeight() * DistanceObstacle()) / TotalFlightTime();

				FVector SocketLocation = SpawnPointTransform.GetLocation();
				FVector SocketRotationForward = SpawnPointTransform.GetRotation().GetForwardVector() * DistanceToReachMaxHeight;
				FVector SocketRotationUp = SpawnPointTransform.GetRotation().GetUpVector() * (RunnerHero->GetMaxJumpHeight() + 30);

				SpawnPointTransform.SetLocation(SocketLocation + SocketRotationForward + SocketRotationUp);
			}
			else
			{
				/// Make distance greater
				FVector SocketLocation = SpawnPointTransform.GetLocation();
				FVector SocketRotationForward = SpawnPointTransform.GetRotation().GetForwardVector() * -DistanceObstacle();
				SpawnPointTransform.SetLocation(SocketLocation + SocketRotationForward);
			}
			//SpawnPointTransform.AddToTranslation(FVector(-DistanceObstacle(), 0, 0));

		}
		 
	}


	PreviousCorridor = GetWorld()->SpawnActor<ACorridor>(
		CorridorToSpawn,
		SpawnPointTransform
		);
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

}

