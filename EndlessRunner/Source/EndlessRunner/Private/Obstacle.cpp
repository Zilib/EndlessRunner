// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "Engine/StaticMesh.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh Component"));
	SetRootComponent(StaticMesh);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule Component"));
	CapsuleCollision->SetRelativeLocation(FVector(0, 0, 100));
	CapsuleCollision->SetCapsuleHalfHeight(170.0f);
	CapsuleCollision->SetCapsuleRadius(130.0f);
	CapsuleCollision->AttachTo(StaticMesh);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

