// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item.generated.h"

class ACorridor;
class UMyGameInstance;
class UStaticMeshComponent;
class UCapsuleComponent;

UCLASS()
class ENDLESSRUNNER_API AItem : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Setup")
	UStaticMeshComponent* StaticMesh{ nullptr };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Setup")
	UCapsuleComponent* CollisionCapsule{ nullptr };

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = "Setup")
	UAudioComponent* SoundToSpawn{ nullptr };

	void SetGameInstance(UMyGameInstance* GameInstanceToSet);
	void SetCorridor(ACorridor* CorridorToSet);
private:
	// Collect a item
	UFUNCTION()
	void OnPickUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UMyGameInstance* GameInstance{	nullptr	};
	ACorridor* AttachedCorridor{ nullptr }; // Tell me, which object want to kill me
};
