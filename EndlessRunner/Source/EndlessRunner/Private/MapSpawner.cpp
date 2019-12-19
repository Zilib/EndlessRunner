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

// Calculate difference in X forward vector
float AMapSpawner::DistanceObstacle()
{
	if (!ensure(RunnerHero)) { return 0; }
	float V0 = RunnerHero->GetV0Velocity();
	// Only add x distance
	// Calculate projectile motion
	// Calculate time to reach ground
	float Td = (2 * V0 * RunnerHero->GetSin()) / 980;
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
		CanTurnLeft = false;
		CanTurnRight = true;

		CorridorToSpawn = *Corridors.Find(FName("Turn Left"));

		// Get position where should be spawned corridor
		SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform("SpawnPointTurnLeft");
	}
	else if (RandomGenerator(ChanceToTurnRight) && CanTurnRight == true)
	{
		CanTurnRight = false;
		CanTurnLeft = true;

		CorridorToSpawn = *Corridors.Find(FName("Turn Right"));

		// Get position where should be spawned corridor
		SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform("SpawnPointTurnRight");
	}
	else
	{
		CanTurnLeft = true;
		CanTurnLeft = true;

		CorridorToSpawn = *Corridors.Find(FName("Straight"));

		// Get position where should be spawned corridor
		 SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform("SpawnPointStraight");
	}
	// If distance hit a good percent, spawn it in another distance
	if (RandomGenerator(ChanceToGreaterDistance))
	{
		FVector NewLocation = FVector(
			SpawnPointTransform.GetLocation().X + (FMath::Abs(SpawnPointTransform.GetRotation().GetForwardVector().X) * DistanceObstacle()),
			SpawnPointTransform.GetLocation().Y + (FMath::Abs(SpawnPointTransform.GetRotation().GetRightVector().X) * DistanceObstacle()),
			SpawnPointTransform.GetLocation().Z
		);
		SpawnPointTransform = FTransform(
			SpawnPointTransform.GetRotation(),
			NewLocation,
			SpawnPointTransform.GetScale3D()
		);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *SpawnPointTransform.GetRotation().GetForwardVector().ToString());
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

