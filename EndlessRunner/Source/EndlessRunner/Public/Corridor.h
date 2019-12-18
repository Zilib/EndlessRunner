// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Coin.h"
#include "Corridor.generated.h"


UCLASS()
class ENDLESSRUNNER_API ACorridor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACorridor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CorridorMesh{ nullptr };

	UPROPERTY(EditAnywhere)
	TArray<FName> SpawnPointNames; // Array of rows and cols

	// This variable is needed because, i need to precice where next object will spawn.
	// Basiclly it is 500.0f thats mean 500cm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CorridorLength{ 500.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CorridorWidth{ 250.0f };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void CreateCollisionBox();

	// After begin overlap wait 3 second and destroy this element. 
	void DestroyObject();

	FTimerHandle Handler;

	void SpawnCoin();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACoin> CoinToSpawn;

	ACoin* SpawnedCoin;
};