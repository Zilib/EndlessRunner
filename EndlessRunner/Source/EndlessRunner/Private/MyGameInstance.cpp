// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Containers/Array.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Misc/FileHelper.h"

// If you do not wanna see a save score btn, or you want to reset statistics use it
void UMyGameInstance::ClearLevelData()
{
	CollectedItems = 0;
	TraveledDistance = 0;
}

// Save player name into a variable
void UMyGameInstance::SetRecordPlayerName(FString PlayerName)
{
	RecordData.PlayerName = PlayerName;
}

// Save score into a variable
void UMyGameInstance::SetRecordScore(int32 Score)
{
	RecordData.Score = Score;
}

// Clear temporary data, i never want to double my results.
void UMyGameInstance::ClearTemporaryArray()
{
	CurrentScoreData.Empty();
}

void UMyGameInstance::IncreaseSpeed()
{
	// If player did not reach max speed.
	if (PlayerSpeed + Acceleration <= MaxPlayerSpeed)
	{
		PlayerSpeed += Acceleration;
	}
}

// In file should be only ip address, so if everything is fine. It will work.
FString UMyGameInstance::GetServerIP() 
{
	FString const Directory = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Data"));
	FString Result;
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();

	if (File.CreateDirectory(*Directory))
	{
		FString const MyFile = Directory + "/config.txt";
		FFileHelper::LoadFileToString(Result, *MyFile);
	}
	return Result;
}