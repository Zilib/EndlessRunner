// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceStructs.generated.h"
/**
 * 
 */

// Here is a data downloaded from sql, so i need alike variables like in the database
USTRUCT(BlueprintType)
struct ENDLESSRUNNER_API FScoreBoard
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int32 Score;
};

// Which kind of menu is currenctly open
UENUM(BlueprintType)
enum class EActiveBoard : uint8
{
	GMainMenu UMETA(DisplayName = "Main Menu"), // Main menu, here player can select what he want to do. 
	GScoreBoard UMETA(DisplayName = "Scoreboard"), // Player can look at the best of 10 players
	GSaveGame UMETA(DisplayName = "Save Score") // He can also save his data to share with community
};