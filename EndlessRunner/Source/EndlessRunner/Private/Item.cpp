// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "MyGameInstance.h"
#include "Corridor.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	SetRootComponent(StaticMesh);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("Box Component"));
	CollisionCapsule->SetRelativeLocation(FVector(.0f, .0f, 340.f));
	CollisionCapsule->InitCapsuleSize(385.f, 814.f);
	CollisionCapsule->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);

	SoundToSpawn = CreateDefaultSubobject<UAudioComponent>(FName("Collect Sound"));
	SoundToSpawn->bAutoActivate = false; // Do not play sound at start the game
	SoundToSpawn->bOverrideAttenuation = true;
	SoundToSpawn->bAutoDestroy = true; 
	SoundToSpawn->bStopWhenOwnerDestroyed = false; // Play after coin destroy
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnPickUp);
}

void AItem::OnPickUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If gameinstance exists and overlapped actor is equal to player hero. Only one hero in the game, so only check his name.
	if (GameInstance && GameInstance->Player == OtherActor)
	{
		// Increase number of collected items
		GameInstance->CollectedItems++;

		SoundToSpawn->Play(); // Coin is collected, give sound!

		AttachedCorridor->SpawnedItem = nullptr; // Tell him, that i am died... He do not must me kill me, because i am died
		
		Destroy(); // Now destroy it, it is not necessary any more
	}
}
// Avoid to cast redundants
void AItem::SetGameInstance(UMyGameInstance* GameInstanceToSet)
{
	GameInstance = GameInstanceToSet;
}

void AItem::SetCorridor(ACorridor* CorridorToSet)
{
	AttachedCorridor = CorridorToSet;
}
