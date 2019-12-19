// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyGameInstance.h"
#include "RunnerCharacter.generated.h"

class UUserWidget;

UCLASS()
class ENDLESSRUNNER_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ARunnerCharacter();
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Return traveled distance in meter
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetTotalDistanceTraveled();

	// Call to this, if you want to change hero director
	UFUNCTION(BlueprintCallable)
	void TurnARunner(float Value);

	// Return from pitagoras an C of triangle. 
	UFUNCTION()
	float GetV0Velocity();

	// Retrun sinus of V0 and Vy 
	UFUNCTION()
	float GetSin();

	// Return cosinus of V0 an Vx
	UFUNCTION()
	float GetCos();
protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

private:
	// It will be showed at screen, as score.
	float TotalDistanceTraveled{ 0.0f };

};