// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "RunnerCharacter.generated.h"

class UMyGameInstance;

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
	
	UMyGameInstance* GameInstance{ nullptr };
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

	/// Getters
	// Map spawner need this value for change level difficulty
	float GetMaxWalkSpeed() const;

	// Return JumpZVelocity
	float GetJumpZVelocity() const;

	// Call to this, if you want to change hero director
	UFUNCTION(BlueprintCallable)
	void TurnARunner(const float Value) const;

	 // Return from pitagoras sqrt(pow(Vx,2) + pow(Vy,2))
	float GetV0Velocity() const; 

	// Retrun sinus of V0 and Vy 
	float GetSin() const; 

	// Return cosinus of V0 and Vx
	float GetCos() const;

	// Return max jump height
	float GetMaxJumpHeight() const;

	// Return time to reach maximum height
	float TimeToReachMaximumHeight() const;

	// Kill a runner
	UFUNCTION(BlueprintCallable)
	void KillARunner();

	void IncreaseSpeed(); // Make level harder, and player will be faster
protected:
	/** Called for forwards/backward input */
	void MoveForward(const float Value);

	/** Called for side to side input */
	void MoveRight(const float Value);
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

	bool CanMoveRight(float Value) const; // Forbid player move into a wall
private:
	// Delay to call function "RestartLevel" to show menu, find how long is boom sound.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DelayToRestart{ 1.0f };

	// Make boom after die
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ImpactBlast{ nullptr };

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* DieSound{ nullptr };
	
	UPROPERTY(EditAnywhere, Category = "Setup")
	float FallingMargin{ 200.0f }; // Margin of character falling, if runner doesn't jump he will be killed when he's getvelocity().z reach jumpZ velocity + falling margin

	void RestartLevel(); // When player die, wait 1.5 sec and restart level
};