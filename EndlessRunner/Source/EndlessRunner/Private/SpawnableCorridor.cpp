// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableCorridor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"

// Sets default values
ASpawnableCorridor::ASpawnableCorridor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
void ASpawnableCorridor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(Handler, this, &ASpawnableCorridor::SpawnCorridor, 0.1f, false);
}

void ASpawnableCorridor::SpawnCorridor()
{
	if (CorridorToSpawn == nullptr) { return; }
	//UE_LOG(LogTemp, Warning, TEXT("%s: Translation"), *GetTransform().ToString());
	FVector NewLocation = FVector(
		GetTransform().GetLocation().X + CorridorWidth,
		GetTransform().GetLocation().Y,
		GetTransform().GetLocation().Z
	);

	FTransform x = FTransform(
		GetTransform().GetRotation(),
		NewLocation,
		GetTransform().GetScale3D()
	);
	
	auto NewCorridor = GetWorld()->SpawnActor<ASpawnableCorridor>(
		CorridorToSpawn,
		x
		);
}