// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MathFunctionsMove.generated.h"


UCLASS()
class ENDLESSRUNNER_API AMathFunctionsMove : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMathFunctionsMove();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* StaticMesh{ nullptr };

	bool isGx{ false }; // G(x) is a function which pattern is abs(x) + sqrt((4 - (pow(x,2)). And F(x) = abs(x) - sqrt((4 - (pow(x,2))

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	float Velocity{ 0.01f };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
