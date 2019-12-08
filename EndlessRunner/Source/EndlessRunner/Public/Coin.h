// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
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

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CollisionCapsule{ nullptr };

private:
	// Collect a item
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
