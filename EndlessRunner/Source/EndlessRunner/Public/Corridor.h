// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Coin.h"
#include "RunnerCharacter.h"
#include "Corridor.generated.h"

class AObstacle;

UCLASS()
class ENDLESSRUNNER_API ACorridor : public AActor
{
	GENERATED_BODY()
	
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

	ACoin* SpawnedItem{ nullptr };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// Basiclly only for destory objects
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// After begin overlap wait 3 second and destroy this element. 
	void DestroyObject();

	// Every will destroy after 1 sec of begin overlap. It handle time of it
	FTimerHandle Handler;

	// Kill a player when he hit a front wall
	UFUNCTION(BlueprintCallable) 
	void KillPlayer(AActor* Player);
};