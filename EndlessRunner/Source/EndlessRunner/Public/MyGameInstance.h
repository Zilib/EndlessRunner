// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString RunnerBPName;

	// Name which will be visible in UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString CollectingItemName{ "Bottle" };

	UPROPERTY(BlueprintReadWrite)
	int32 CollectedItem{ 0 };
};
