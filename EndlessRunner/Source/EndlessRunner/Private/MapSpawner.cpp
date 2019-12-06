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
	
	if (!ensure(StartPlatform)) { return; }
	if (!ensure(CorridorToSpawn)) { return; }

	LastCorridor = GetWorld()->SpawnActor<ASpawnableCorridor>(
		CorridorToSpawn,
		StartPlatform->GetStaticMeshComponent()->GetSocketTransform(FName("SpawnPoint"))
		);
	
}
// Called every frame
void AMapSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Before game start, generate 100 corridors, cuz player will see generate proces
	if (!GetWorldTimerManager().IsTimerActive(CreateCorridorHandler) && SpawnedCorridors < 5)
	{
		int32 RandomNumber = FMath::RandRange(2, 5);


		if ((SpawnedCorridors + 1) % RandomNumber == 0)
		{
			SpawnNextCorridor(
				StaticMeshes.FindRef(FName("Corner")),
				true
			);
		}
		else if ((SpawnedCorridors + 1) % RandomNumber != 0)
		{
			SpawnNextCorridor(
				StaticMeshes.FindRef(FName("Straight")),
				false
			);
		}
		SpawnedCorridors++;
	}
	else if (SpawnedCorridors == 100 && !GetWorldTimerManager().IsTimerActive(CreateCorridorHandler))
	{
		UE_LOG(LogTemp, Warning, TEXT("Udalo sie! Brawo"));
		SpawnedCorridors++;
	}
}
void AMapSpawner::SpawnNextCorridor(UStaticMesh* MeshToSet, bool isCorner)
{
	if (!ensure(StartPlatform)) { return; }
	if (!ensure(CorridorToSpawn)) { return; }
	LastCorridor = GetWorld()->SpawnActor<ASpawnableCorridor>(
		CorridorToSpawn,
		LastCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPoint"))
		); 
	LastCorridor->SetMesh(MeshToSet, isCorner);

}

