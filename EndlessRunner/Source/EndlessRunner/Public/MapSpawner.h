// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnableCorridor.h"
#include "Corridor.h"
#include "TimerManager.h"
#include "MapSpawner.generated.h"

class AStaticMeshActor;
class ACorridor;

UCLASS()
class ENDLESSRUNNER_API AMapSpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Here i will hold a new object, for get next spawn location.
	// Firstly you need to give me a first object, because i do not know where should i spawn next object
	// Basicly it would be a start platform.
	UPROPERTY(EditAnywhere)
	ACorridor* CorridorToGetTransform{ nullptr };

	bool IsMapGenerated{ false }; // Map is generated when 100 corridors is created, after it create everything slowely
	int32 SpawnedCorridors{ 0 };

	FTimerHandle CreateCorridorHandler;

public:	
	// Sets default values for this actor's properties
	AMapSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnNextCorridor(UStaticMesh* MeshToSet, bool isCorner);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ACorridor>> Corridors;

};
