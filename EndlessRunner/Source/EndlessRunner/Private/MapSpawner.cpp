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
	
	if (!ensure(CorridorToGetTransform)) { return; }
	for (int i = 0; i < 10; i++)
	{
		FTransform SocketTransform = CorridorToGetTransform->CorridorMesh->GetSocketTransform(FName("SpawnPoint"));
		FVector NewLocation = FVector(
			SocketTransform.GetLocation().X + (CorridorToGetTransform->CorridorLength / 2),
			SocketTransform.GetLocation().Y,
			SocketTransform.GetLocation().Z
		);

		FTransform NewTransform = FTransform(
			SocketTransform.GetRotation(),
			NewLocation,
			SocketTransform.GetScale3D());
		CorridorToGetTransform = GetWorld()->SpawnActor<ACorridor>(
			Corridors[0],
			NewTransform
			);
	}
//	if (!ensure(CorridorToSpawn)) { return; }

	
}
// Called every frame
void AMapSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Before game start, generate 100 corridors, cuz player will see generate proces
/*	if (!GetWorldTimerManager().IsTimerActive(CreateCorridorHandler) && SpawnedCorridors < 100)
	{
		int32 RandomNumber = FMath::RandRange(1, 30);
		LastCorridor = GetWorld()->SpawnActor<ACorridor>(
			(SpawnedCorridors+1) % 50 == 0 ? Corridors[1] : Corridors[0],
			LastCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPoint"))
			);
		SpawnedCorridors++;
	}
	else if (SpawnedCorridors == 100 && !GetWorldTimerManager().IsTimerActive(CreateCorridorHandler))
	{
		UE_LOG(LogTemp, Warning, TEXT("Udalo sie! Brawo"));
		SpawnedCorridors++;
	}*/
}
void AMapSpawner::SpawnNextCorridor(UStaticMesh* MeshToSet, bool isCorner)
{
//	LastCorridor = GetWorld()->SpawnActor<ACorridor>(
	//	CorridorToSpawn,
	//	LastCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPoint"))
	//	); 

}

