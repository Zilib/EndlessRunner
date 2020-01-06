// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Item.h"
#include "Obstacle.h"
#include "MyGameInstance.h"
#include "RunnerCharacter.h"
#include "Corridor.generated.h"

class AItem;

UCLASS()
class ENDLESSRUNNER_API ACorridor : public AActor
{
	GENERATED_BODY()

	UMyGameInstance* GameInstance{ nullptr };
public:	
	// Sets default values for this actor's properties
	ACorridor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* TriggerDestroy{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CorridorMesh{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FName> SpawnPointNames; // Array of names rows and cols

	// Only keep to destroy this in the future. Destroy it with corridor
	AObstacle* SpawnedObstacleRock{ nullptr };

	AItem* SpawnedItem{ nullptr };
	// After begin overlap wait 3 second and destroy this element. 
	void DestroyObject();
protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	UFUNCTION()
	virtual void DestroyOnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	// Time untill destroy an object
	FTimerHandle TimeToDestroy;
	// Kill a player when he hit a front wall
	UFUNCTION(BlueprintCallable) 
	void KillPlayer(AActor* OverlappedActor) const;

	bool IsDestroyTimerActive{ false };
};