// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "CollectSound.h"
#include "Coin.generated.h"


UCLASS()
class ENDLESSRUNNER_API ACoin : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay();

public:	
	// Sets default values for this actor's properties
	ACoin();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Setup")
	UStaticMeshComponent* StaticMesh{ nullptr };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Setup")
	UCapsuleComponent* CollisionCapsule{ nullptr };

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = "Setup")
	UAudioComponent* SoundToSpawn{ nullptr };
private:
	// Collect a item
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
