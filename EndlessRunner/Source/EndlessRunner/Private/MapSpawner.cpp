// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSpawner.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
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

}
void AMapSpawner::SpawnNextCorridor(UStaticMesh* MeshToSet)
{
	if (!ensure(StartPlatform)) { return; }
	if (!ensure(CorridorToSpawn)) { return; }
	LastCorridor = GetWorld()->SpawnActor<ASpawnableCorridor>(
		CorridorToSpawn,
		LastCorridor->CorridorMesh->GetSocketTransform(FName("SpawnPoint"))
		); 
	LastCorridor->SetMesh(MeshToSet);
}

