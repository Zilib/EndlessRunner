// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "SpawnableCorridor.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENDLESSRUNNER_API ASpawnableCorridor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnableCorridor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CorridorMesh{ nullptr };

	UFUNCTION(BlueprintCallable)
	void SetMesh(UStaticMesh* MeshToSet);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Setup")
	float CorridorWidth{ 600.0f };

	FTimerHandle Handler;


};
