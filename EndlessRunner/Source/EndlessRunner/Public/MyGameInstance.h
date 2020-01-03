// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InterfaceStructs.h" 
#include "RunnerCharacter.h"
#include "MyGameInstance.generated.h"


class ARunnerCharacter;
/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// use this variable to change level difficulty, after overlapp corridor, add it.
	float Acceleration{ 10.0f }; 

	float MaxPlayerSpeed{ 600 }; 

	float PlayerSpeed{ 200 }; // Player speed when reach a corridor which will be spawned

	void IncreaseSpeed();

	// Clear all variables, prepare level for start
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void ClearLevelData();

	// Save here player character, whole game is communicating thankfully by this instance
	ARunnerCharacter* Runner{ nullptr };

	// Name which will be visible in UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	FString CollectingItemName{ "Bottle" };

	// When player get a item, collect it here!
	UPROPERTY(BlueprintReadWrite, Category = "Game Statistics")
	int32 CollectedItem{ 0 }; 

	// When player is running he is making an distance! Save here his last done distance.
	UPROPERTY(BlueprintReadWrite, Category = "Game Statistics")
	int32 TraveledDistance{ 0 };  
		
	// Add multiply of it and collected items into score
	UPROPERTY(VisibleAnywhere, Category = "Game Statistics")
	int32 ItemValue{ 100 }; 

	// Pattern: TraveledDistance / 33 + CollectedItem * ItemValue
	UFUNCTION(BlueprintCallable, Category = "Game Statistics")
	FORCEINLINE int32 GetScore() const { return (TraveledDistance / 33) + CollectedItem * ItemValue; }

	// Return traveled distance in meters
	UFUNCTION(BlueprintCallable, Category = "Game Statistics")
	FORCEINLINE int32 GetTotalDistanceTraveled() const { return TraveledDistance / 1000; };

	// Main interface, which menu is actually active.
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	EActiveBoard ActiveBoard{ EActiveBoard::GMainMenu };
/// Setters
	// Save temporary player name to then save it to proper variable
	UFUNCTION(BlueprintCallable, Category = "TData")
	void SetPlayerName(FString PlayerName);

	// Save temporary temporary score to then save it to proper variable
	UFUNCTION(BlueprintCallable, Category = "TData")
	void SetScore(int32 Score);

	// Clear temporary array, it is a prepare before get a new data.
	UFUNCTION(BlueprintCallable, Category = "TData")
	void ClearTemporaryArray();

	// Now save all data into a array
	UFUNCTION(BlueprintCallable)
	void AddToData();

/// Getters 
	// Get a copy of FScoreBoard table, to show it on the screen
	UFUNCTION(BlueprintCallable, Category = "TData")
	FORCEINLINE TArray<FScoreBoard> GetData() { return Data; }

	// Get game version
	UFUNCTION(BlueprintCallable, Category = "Setup")
	FORCEINLINE	FString GetGameVersion() const { return GameVersion; }

	// Call into file, which is into data directory and is named "config.txt"
	UFUNCTION(BlueprintCallable, Category = "Server")
	FString GetServerIP();
private:
	// Save here data which will be saved into Data array
	FScoreBoard TemporaryData; 

	// Here are stored table records, which you can see after click scoreboard.
	TArray<FScoreBoard> Data; 

	// Set game version, can be changed only in the code.
	FString GameVersion{ "1.0" }; 
};
