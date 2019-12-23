// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Corridor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AEndlessRunnerCharacter

ARunnerCharacter::ARunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set character name, to allow check other comp does they collised with your hero
	Cast<UMyGameInstance>(GetGameInstance())->RunnerBPName = GetName();
}

void ARunnerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MoveForward(1);
	// Increase traveled distance 
	TotalDistanceTraveled += FVector::DotProduct(GetVelocity(), GetActorRotation().Vector()) / 100;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveRight", this, &ARunnerCharacter::MoveRight);
}

void ARunnerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARunnerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

// Check does player can turn right, he cannot run into a wall
bool ARunnerCharacter::CanMoveRight(float Value)
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors, ACorridor::StaticClass());
	if (Actors.Num() > 0)
	{
		for (const auto& Actor : Actors)
		{
			// Corridor where actually player is.
			ACorridor* pCurrentCorridor = Cast<ACorridor>(Actor);
			if (pCurrentCorridor)
			{
				// Get walls from corridor
				TArray<UActorComponent*> Walls = pCurrentCorridor->GetComponentsByTag(UBoxComponent::StaticClass(), FName("Wall"));
				if (Walls.Num() > 0)
				{
					for (const auto& Wall : Walls)
					{
						UBoxComponent* WallComponent = Cast<UBoxComponent>(Wall);
						// Check for direction
						// If value > 0 runner is trying to turn right
						if (Value > 0
						&& WallComponent->ComponentHasTag(FName("RightWall")) // Get left wall component
						&& WallComponent->IsOverlappingComponent(GetCapsuleComponent()) ) // If player is overlapping with it, forbid him turning
						{
							return false;
						}
						else if (Value < 0 // If value is less than 0, runner is trying to go left
						&& WallComponent->ComponentHasTag(FName("LeftWall")) // Get left wall component
						&& WallComponent->IsOverlappingComponent(GetCapsuleComponent()) ) // If player is overlapping with it, forbid him turning
						{
							return false;
						}
					}
				}
			}
		}

	}
	return true;
}
void ARunnerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && CanMoveRight(Value))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// Change a runner directory while he is running. Using only if you make an turn.
void ARunnerCharacter::TurnARunner(float Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator NewRotation = FRotator(
		Rotation.Pitch,
		Rotation.Yaw + Value,
		Rotation.Roll
	);
	Controller->SetControlRotation(NewRotation);
}

// Return traveled distance in meters
int32 ARunnerCharacter::GetTotalDistanceTraveled() const
{
	return (int32)TotalDistanceTraveled / 1000;
}

// Return an V0 velocity, it is necessary for calculate projectile motion
float ARunnerCharacter::GetV0Velocity() const
{
	return FMath::Sqrt(
		FMath::Pow(GetCharacterMovement()->JumpZVelocity, 2) + FMath::Pow(GetCharacterMovement()->MaxWalkSpeed, 2)
	);
}

// Return an sinus of JumpZ velocity and V0(Vy and V0). Necessary for projectile motion
float ARunnerCharacter::GetSin() const
{
	return GetCharacterMovement()->JumpZVelocity / GetV0Velocity();
}

// Return an cosinus of MaxWalkSpeed velocity and V0(Vx and V0). Necessary for projectile motion
float ARunnerCharacter::GetCos() const
{
	return GetCharacterMovement()->MaxWalkSpeed / GetV0Velocity();
}

// Return max jump height
float ARunnerCharacter::GetMaxJumpHeight() const
{
	return GetCharacterMovement()->GetMaxJumpHeight();
}

// Return time to reach max jump height
float ARunnerCharacter::TimeToReachMaximumHeight() const
{
	return (GetV0Velocity() * GetSin()) / GetWorld()->GetGravityZ();
}