// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSpawner.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
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

	auto ToShow = PreviousCorridor->GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("%s dsadsadas"), *ToShow.ToString());

	GetWorld()->GetTimerManager().SetTimer(hTimer, this, &AMapSpawner::SpawnCorridor, 1.0f, true);
}

void AMapSpawner::SpawnCorridor()
{
	// Get position where should be spawned corridor
	FTransform SpawnPointTransform = PreviousCorridor->CorridorMesh->GetSocketTransform("SpawnPoint");

	// Spawn turn left corridor
	if (RandomGenerator(ChanceToTurnLeft) && CanTurnLeft == true)
	{
		CanTurnLeft = false;
		CanTurnRight = CanTurnRight == true;

		TSubclassOf<ACorridor> CorridorToSpawn = *Corridors.Find(FName("Turn Left"));


		FVector NewLocation = FVector(
			SpawnPointTransform.GetLocation().X + (CorridorToSpawn.GetDefaultObject()->CorridorLength / 2),
			SpawnPointTransform.GetLocation().Y + (CorridorToSpawn.GetDefaultObject()->CorridorWidth / 2),
			SpawnPointTransform.GetLocation().Z
		);

		FTransform NewTransform = FTransform(
			SpawnPointTransform.GetRotation(),
			NewLocation,
			SpawnPointTransform.GetScale3D()
		);

		PreviousCorridor = GetWorld()->SpawnActor<ACorridor>(
			*Corridors.Find(FName("Turn Left")),
			NewTransform
		); 
	}
}

// Random system, to make some obstacles.
bool AMapSpawner::RandomGenerator(int Chance)
{
	int32 RandomNumber = FMath::FRandRange(1, 100);

	return RandomNumber < Chance;
}

// Called every frame
void AMapSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

