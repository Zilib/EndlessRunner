// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Corridor.h"
#include "TimerManager.h"
#include "MapSpawner.generated.h"

class ACorridor;
class ARunnerCharacter;
class AObstacle;

UCLASS()
class ENDLESSRUNNER_API AMapSpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	int32 SpawnedCorridors{ 0 };

	// Need for calculate maximum jump distance and height. For estimate an obscales difficulty level
	UPROPERTY(EditAnywhere)
	ARunnerCharacter* RunnerHero{ nullptr };

	// Here i will hold a new object, for get next spawn location.
	// Firstly you need to give me a first object, because i do not know where should i spawn next object
	// Basicly it would be a start platform.
	UPROPERTY(EditAnywhere)
	ACorridor* PreviousCorridor{ nullptr };

	UFUNCTION()
	void GenerateMap();

	bool IsMapGenerated{ false }; // Map is generated when 100 corridors is created, after it create everything slowely

	// Keep time between spawning corridors.
	FTimerHandle hTimer;

	// Try to avoid sytuation where corridors will make a square, it will never can make twice this same kind of turn corridor.
	bool CanTurnLeft{ true };
	bool CanTurnRight{ true };

	/// Randomize map
	// In percent, it determines chance to make a space between next corridor, which mean that a player have to jump over it.
	UPROPERTY(EditAnywhere, Category = "Chances setup")
	int32 ChanceToGreaterDistance{ 25 };

	// It will only possibile if GreaterDistanceHit, chance to spawn it at max high distance
	UPROPERTY(EditAnywhere, Category = "Chances setup")
	int32 ChanceToJump{ 10 };

	// Chance to spawn turn left corridor
	UPROPERTY(EditAnywhere, Category = "Chances setup")
	int32 ChanceToTurnLeft{ 10 };

	// Chance to spawn turn right corridor
	UPROPERTY(EditAnywhere, Category = "Chances setup")
	int32 ChanceToTurnRight{ 10 };

	// A function which will check and make random function.
	bool RandomGenerator(int Chance);

	// Calculate where should be spawned next corridor, if runner have to jump. Use projectile motion pattern
	FORCEINLINE float GetDisplacement(float V0, float t, float Cos0) const { return V0 * t* Cos0; }

	// It is constant value which mean what the corridor displacement is. Actually is not to change, in future it will not const, and displacement will be dependendent of player speed. Actually Runner speed is also constant
	float CorridorDisplacement;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float DisplacementMarginValue{ 0 }; // How much cm corridor should be spawned before, it will make jump easier. 

	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 StartToSpawnCorridor{ 20 }; // When map will spawned this number of corridors, game will change delay between spawn next corridor

	UPROPERTY(EditAnywhere, Category="Setup")
	float NextDelay{ 0.5f }; // Next delay, after script spawn StartToSpawnCorridor value numbers of corridor.

	UPROPERTY(EditAnywhere, Category= "Setup")
	int32 WaitCorridors{ 3 }; // Don't spawn rocks before spawn X corridors

	bool LastWasTurn{ false }; // Here is a variable which tell me does last corridor was a turn
	bool NextDelayTimeActive{ false };
	// Return total jump time
	float TotalFlightTime() const;

	/// Spawn functions
	void SpawnTurnRightCorridor();
	void SpawnTurnLeftCorridor();
	void SpawnStraightCorridor();
	void SpawnCorridor(); // Common function, called from every previous functions to increase number of spawned corridors and to spawn corridor.
	// Spawn rock
	void SpawnObstacleRock();

	FTransform SpawnPointTransform; // Next spawn point location
	TSubclassOf<ACorridor> CorridorToSpawn = NULL; // What kind of corridor will be spawned next

	void SpawnItem(); // Spawn coin

	// Item spawner
	UPROPERTY(EditAnywhere, Category = "Item spawner")
	TSubclassOf<ACoin> ItemToSpawn; // Select "coin" class

	// Multiply every collected bottle by this value and add it to score
	UPROPERTY(EditAnywhere, Category = "Item spawner")
	int32 ItemValue {100};
public:	
	// Sets default values for this actor's properties
	AMapSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Straight, Turn etc...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName,TSubclassOf<ACorridor>> Corridors;

	// Give me here a blueprint of obstacle to spawn!
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AObstacle> ObstacleBP;

};
