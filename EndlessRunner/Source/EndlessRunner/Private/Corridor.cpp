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
	if (UMyGameInstance * GameInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GameInstance->IncreaseSpeed();
	}
}

void ACorridor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<UMyGameInstance>(GetGameInstance())) { return; }

	// Increase player speed after reach corridor
	if (ARunnerCharacter* Player = Cast<ARunnerCharacter>(OtherActor))
	{
		// Make player run faster
		Player->IncreaseSpeed();
	}

	// If collision is with hero
	if (OtherActor->GetName() == Cast<UMyGameInstance>(GetGameInstance())->RunnerBPName)
	{
		FTimerHandle TimeToDestroy;
		// set timer and after X time destroy object, and spawned items thankfully this object.
		GetWorldTimerManager().SetTimer(TimeToDestroy, this, &ACorridor::DestroyObject, 4.0f, false);
	}
}

// Kill a player when he hit a front wall
void ACorridor::KillPlayer(AActor* Player)
{
	if (ARunnerCharacter* Runner = Cast<ARunnerCharacter>(Player))
	{
		// If player cannot jump, he is in the air. So he hit the wall during he jump. That is a reason to kill him! And forbidd play anymore
		if (!Runner->CanJump()) 
		{
			Runner->KillARunner();
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
	if (IsValid(SpawnedItem))
	{
		SpawnedItem->Destroy();
	}
	if (this)
	{
		Destroy();
	}
}
