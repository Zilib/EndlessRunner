// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameStructsAndEnums.h"
#include "SecureCPP.h"
#include "RunnerCharacter.h"
#include "MyGameInstance.generated.h"

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
	ARunnerCharacter* Player{ nullptr };

	// Name which will be visible in UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	FString CollectingItemName{ "Bottle" };

	// When player get a item, collect it here!
	UPROPERTY(BlueprintReadWrite, Category = "Game Statistics")
	int32 CollectedItems{ 0 }; 

	// When player is running he is making an distance! Save here his last done distance.
	UPROPERTY(BlueprintReadWrite, Category = "Game Statistics")
	int32 TraveledDistance{ 0 };  
		
	// Add multiply of it and collected items into score
	UPROPERTY(VisibleAnywhere, Category = "Game Statistics")
	int32 ItemValue{ 100 }; 

	// Pattern: TraveledDistance / 33 + CollectedItem * ItemValue
	UFUNCTION(BlueprintCallable, Category = "Game Statistics")
	FORCEINLINE int32 GetScore() const { return (TraveledDistance / 33) + CollectedItems * ItemValue; }

	// Return traveled distance in meters
	UFUNCTION(BlueprintCallable, Category = "Game Statistics")
	FORCEINLINE int32 GetTotalDistanceTraveled() const { return TraveledDistance / 1000; };

	// Main interface, which menu is actually active.
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	EActiveBoard ActiveBoard{ EActiveBoard::GMainMenu };
/// Setters
	// Save temporary player name to then save it to proper variable
	UFUNCTION(BlueprintCallable, Category = "Scoreboard Data")
	void SetRecordPlayerName(FString PlayerName);

	// Save temporary temporary score to then save it to proper variable
	UFUNCTION(BlueprintCallable, Category = "Scoreboard Data")
	void SetRecordScore(int32 Score);

	// Clear temporary array, it is a prepare before get a new data.
	UFUNCTION(BlueprintCallable, Category = "Scoreboard Data")
	void ClearTemporaryArray();

	// Here are stored table records, which you can see after click scoreboard.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard Data")
	TArray<FScoreBoard> CurrentScoreData;

	// Save here data which will be saved into Data array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard Data")
	FScoreBoard RecordData;
/// Getters 
	// Get game version
	UFUNCTION(BlueprintCallable, Category = "Setup")
	FORCEINLINE	FString GetGameVersion() const { return GameVersion; }
	
	// Call into file, which is into data directory and is named "config.txt"
	UFUNCTION(BlueprintCallable, Category = "Server")
	static FString GetServerIP();
private:
	// Set game version, can be changed only in the code.
	FString GameVersion{ "1.0" };

	// Here are every information to connect with database send there data. SecureCPP is will be not send to github
	UFUNCTION(BlueprintCallable, Category = "Database data")
	FORCEINLINE FString GetUsernameFieldValue() const { return DBInformation.UsernameFieldValue; }

	UFUNCTION(BlueprintCallable, Category = "Database data")
	FORCEINLINE FString GetPasswordFieldValue() const { return DBInformation.PasswordFieldValue; }

	UFUNCTION(BlueprintCallable, Category = "Database data")
	FORCEINLINE FString GetPasswordFieldName() const { return DBInformation.PasswordFieldName; }
	
	UFUNCTION(BlueprintCallable, Category = "Database data")
	FORCEINLINE FString GetUsernameFieldName() const { return DBInformation.UsernameFieldName; }

	UFUNCTION(BlueprintCallable, Category = "Database data")
	FORCEINLINE FString ScoreBoardURL() const { return DBInformation.GameScoreBoardURL; }

	UFUNCTION(BlueprintCallable, Category = "Database data")
	FORCEINLINE FString SaveURL() const { return DBInformation.GameSaveDataURL; }

	UFUNCTION(BlueprintCallable, Category = "Database data")
	FORCEINLINE FString GetVersionUrl() const { return DBInformation.GameVersionURL; }
	SecureCPP DBInformation;
};
