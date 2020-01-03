// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyGameInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
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

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	DieSound = CreateDefaultSubobject<UAudioComponent>(FName("Die Sound"));
	DieSound->bAutoActivate = false; // Do not play at start
	DieSound->bAllowSpatialization = false; // It must play doesnt matter which ear.
	DieSound->bStopWhenOwnerDestroyed = false; // Play after die

}

void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	// Tell instance that you are an hero!
	if (GameInstance)
	{
		GameInstance->Runner = this;
		GameInstance->PlayerSpeed = GetCharacterMovement()->MaxWalkSpeed; // Save start player speed
	}
	// Because game is not started, player cannot move
	GetCharacterMovement()->SetActive(false);
}

void ARunnerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	MoveForward(1);

	// Increase traveled distance 
	Cast<UMyGameInstance>(GetGameInstance())->TraveledDistance += FVector::DotProduct(GetVelocity(), GetActorRotation().Vector()) / 100;

	// If character is going down, he cannot reach more speed than JumpZVelocity, each one obstacle is not spawn below him
	if (abs(GetVelocity().Z) > GetCharacterMovement()->JumpZVelocity + FallingMargin)
	{
		KillARunner();
	}
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

void ARunnerCharacter::TurnAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARunnerCharacter::MoveForward(const float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
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
bool ARunnerCharacter::CanMoveRight(float Value) const
{
	TArray<AActor*> Actors; // Here is saved a overlapped actors
	GetOverlappingActors(Actors, ACorridor::StaticClass());
	if (Actors.Num() > 0) // If the array is empty, it means that mean runner does not overlap anything
	{
		for (const auto& Actor : Actors) // For every overlaped  actor
		{
			// Corridor where actually player is.
			ACorridor* pCurrentCorridor = Cast<ACorridor>(Actor); // Get corridor
			if (pCurrentCorridor) // If it is a corridor
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

void ARunnerCharacter::MoveRight(const float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && CanMoveRight(Value))
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
void ARunnerCharacter::TurnARunner(const float Value) const
{
	if (Controller) // Turn a character, who is controlled by player. 
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator NewRotation = FRotator(
			Rotation.Pitch,
			Rotation.Yaw + Value,
			Rotation.Roll
		);
		Controller->SetControlRotation(NewRotation);
	}
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
// Necessary for map spawner to change level difficulty
float ARunnerCharacter::GetMaxWalkSpeed() const
{
	return GetCharacterMovement()->MaxWalkSpeed;
}
// Kill a runner, make boom restart statistics, and go into a main menu.
void ARunnerCharacter::KillARunner()
{
	if (GetController())
	{
		Cast<UActorComponent>(GetMesh())->DestroyComponent(); // Destroy skeletal mesh, it doesn't look good when player died, but his hero is still alive.
		ImpactBlast->SetActive(true); // Make boom there where were an player
		GetCharacterMovement()->SetActive(false); // When player cannot move he is die.
		if (DieSound)
		{
			DieSound->Play(); // Make boom after die
		}
		FTimerHandle TimeToRestartLevel;
		GetWorld()->GetTimerManager().SetTimer(TimeToRestartLevel, this, &ARunnerCharacter::RestartLevel, DelayToRestart, false);
	}
}

float ARunnerCharacter::GetJumpZVelocity() const
{
	return GetCharacterMovement()->JumpZVelocity;
}

// Increase player speed by acceleration value
void ARunnerCharacter::IncreaseSpeed()
{
	if (UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		// Increase playerspeed by Acceleration defined in the game instance
		if (GetCharacterMovement()->MaxWalkSpeed + GInstance->Acceleration <= GInstance->MaxPlayerSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed += GInstance->Acceleration;
		}
	}
}

void ARunnerCharacter::RestartLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level1")); // Now restart everything
}