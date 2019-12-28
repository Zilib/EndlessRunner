// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Engine/World.h"
#include "MyGameInstance.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	SetRootComponent(StaticMesh);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("Box Component"));
	CollisionCapsule->SetRelativeLocation(FVector(.0f, .0f, 340.f));
	CollisionCapsule->InitCapsuleSize(385.f, 814.f);
	CollisionCapsule->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnOverlapBegin);
}

void ACoin::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName() == Cast<UMyGameInstance>(GetGameInstance())->RunnerBPName)
	{
		Cast<UMyGameInstance>(GetGameInstance())->CollectedItem++;
		Destroy();
	}
}