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

	CorridorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Corridor"));
	SetRootComponent(CorridorMesh);

	CreateCollisionBox();
}

void ASpawnableCorridor::CreateCollisionBox()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
	CollisionBox->AttachTo(CorridorMesh);
	CollisionBox->SetRelativeLocation(FVector(-89.0f, 0.0f, 70.0f));
	CollisionBox->SetRelativeScale3D(FVector(1.0f, 8.5f, 1.0f));
}

void ASpawnableCorridor::BeginPlay()
{
	Super::BeginPlay();

//	GetWorldTimerManager().SetTimer(Handler, this, &ASpawnableCorridor::SpawnCorridor, 0.1f, false);
}

void ASpawnableCorridor::SetMesh(UStaticMesh* MeshToSet,bool isCorner)
{
	CorridorMesh->SetStaticMesh(MeshToSet);
	this->isCorner = isCorner;
}
