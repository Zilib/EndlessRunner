// Fill out your copyright notice in the Description page of Project Settings.


#include "MathFunctionsMove.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values
AMathFunctionsMove::AMathFunctionsMove()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AMathFunctionsMove::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMesh->SetWorldLocation(FVector(2, 2, 0));
}

// Called every frame
void AMathFunctionsMove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentLocation = StaticMesh->GetRelativeTransform().GetLocation();
	if (!isGx)
	{
		
		if (CurrentLocation.X + Velocity > 2.0f)
		{
			isGx = true;
		}
		float NextZPosition = FMath::Abs(CurrentLocation.X + Velocity) + FMath::Sqrt((4 - FMath::Pow(CurrentLocation.X + Velocity, 2)));
		FVector NextLocation = FVector(
			CurrentLocation.X + Velocity,
			NextZPosition,
			CurrentLocation.Y
		);
		StaticMesh->SetRelativeLocation(NextLocation);
	}
	else if (isGx)
	{
		if (CurrentLocation.X - Velocity < -2.0f)
		{
			isGx = false;
		}
		float NextZPosition = FMath::Abs(CurrentLocation.X - Velocity) - FMath::Sqrt((4 - FMath::Pow(CurrentLocation.X - Velocity, 2)));
		FVector NextLocation = FVector(
			CurrentLocation.X - Velocity,
			NextZPosition,
			CurrentLocation.Y
		);
		StaticMesh->SetRelativeLocation(NextLocation);
	}
}

