// Fill out your copyright notice in the Description page of Project Settings.


#include "Corridor.h"
#include "TimerManager.h"
#include "MyGameInstance.h"
#include "Coin.h"
#include "Obstacle.h"
#include "Engine/StaticMesh.h"

// Sets default values
ACorridor::ACorridor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CorridorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Corridor"));
	SetRootComponent(CorridorMesh);
	
	TriggerDestroy = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
	TriggerDestroy->AttachToComponent(CorridorMesh, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ACorridor::BeginPlay()
{
	Super::BeginPlay();
	TriggerDestroy->OnComponentBeginOverlap.AddDynamic(this, &ACorridor::OnOverlapBegin);

	// Tell game, player will be faster, so game must know how fast player will when he will reach next corridor to change displacement
	GameInstance = Cast<UMyGameInstance>(GetGameInstance());

	if (GameInstance)
	{
		GameInstance->IncreaseSpeed();
	}
}

void ACorridor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GameInstance) { return; }

	if (GameInstance && GameInstance->Runner == OtherActor)
	{
		// So player should move faster, after reach corridor
		GameInstance->Runner->IncreaseSpeed();

		// set timer and after X time destroy object, and spawned items thankfully this object.
		if(!IsTimerActive)
		{
			IsTimerActive = true;
			GetWorldTimerManager().SetTimer(TimeToDestroy, this, &ACorridor::DestroyObject, 4.0f, false);
		}
	}
}

// Kill a player when he hit a front wall
void ACorridor::KillPlayer(AActor* Player)
{
	if (Player == GameInstance->Runner) // If overlapped actor it is a player
	{
		// If player cannot jump, he is in the air. So he hit the wall during he jump. That is a reason to kill him! And forbidd play anymore
		if (!GameInstance->Runner->CanJump()) 
		{
			GameInstance->Runner->KillARunner(); // We know, runner overlapped so we can use pointer to get his position in memory and destroy him
		}
	}
}

void ACorridor::DestroyObject()
{
	// if spawned coin exists, and you cannot collect it any more. Just destroy it
	if (IsValid(SpawnedObstacleRock))
	{
		SpawnedObstacleRock->Destroy();
	}
	if (IsValid(SpawnedItem)) // If item is not collected
	{
		SpawnedItem->Destroy();
	}
	if (IsValid(this))
	{
		Destroy();
	}
}