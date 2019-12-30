// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Containers/Array.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Misc/FileHelper.h"

// Save player name into a variable
void UMyGameInstance::SetPlayerName(FString PlayerName)
{
	TemporaryData.PlayerName = PlayerName;
}

// Save traveled meters into a variable
void UMyGameInstance::SetTraveledMeters(FString TraveledMeters)
{
	TemporaryData.TraveledMeters = FCString::Atoi(*TraveledMeters);
}

// Save collected items into a variable
void UMyGameInstance::SetCollectedItems(FString CollectedItems)
{
	TemporaryData.CollectedItems = FCString::Atoi(*CollectedItems);
}
// Clear temporary data, i never want to double my results.
void UMyGameInstance::ClearTemporaryArray()
{
	Data.Empty();
}

void UMyGameInstance::AddToData()
{
	Data.Add(TemporaryData);
}

// In file should be only ip address, so if everything is fine. It will work.
FString UMyGameInstance::GetServerIP()
{
	FString directory = FPaths::Combine(FPaths::GameContentDir(), TEXT("Data"));
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/config.txt";
		FFileHelper::LoadFileToString(result, *myFile);
	}
	return result;
}