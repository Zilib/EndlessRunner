// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InterfaceStructs.h" 
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// To check does overlap actor is main hero
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Setup")
	FString RunnerBPName;

	// Name which will be visible in UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	FString CollectingItemName{ "Bottle" };

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 CollectedItem{ 0 }; // When player get a item, collect it here!

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 LastTraveledDistance{ 0 };  // When player is running he is making an distance! Save here his last done distance.

/// Setters
	// Save temporary player name to then save it to proper variable
	UFUNCTION(BlueprintCallable, Category = "TData")
	void SetPlayerName(FString PlayerName);

	// Save temporary temporary meters to then save it to proper variable
	UFUNCTION(BlueprintCallable, Category = "TData")
	void SetTraveledMeters(FString TraveledMeters);

	// Save temporary collected items to then save it to proper variable
	UFUNCTION(BlueprintCallable, Category = "TData")
	void SetCollectedItems(FString CollectedItems);

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

private:
	FScoreBoard TemporaryData; // Save here data which will be saved into Data array

	TArray<FScoreBoard> Data; 

	FString GameVersion{ "1.0" }; // Set game version, can be changed only in the code.
};
