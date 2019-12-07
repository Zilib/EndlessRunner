// Fill out your copyright notice in the Description page of Project Settings.


#include "Corridor.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"

// Sets default values
ACorridor::ACorridor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CorridorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Corridor"));
	SetRootComponent(CorridorMesh);

	CreateCollisionBox();
}

// Called when the game starts or when spawned
void ACorridor::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACorridor::OnOverlapBegin);
}

void ACorridor::CreateCollisionBox()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
	CollisionBox->AttachTo(CorridorMesh);
	CollisionBox->SetRelativeLocation(FVector(-89.0f, 0.0f, 70.0f));
	CollisionBox->SetRelativeScale3D(FVector(1.0f, 8.5f, 1.0f));
}

void ACorridor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	FTimerHandle TimeToDestroy;
	GetWorldTimerManager().SetTimer(TimeToDestroy, this, &ACorridor::DestroyObject, 3.0f, false);
}

void ACorridor::DestroyObject()
{
	
	Destroy();
}

