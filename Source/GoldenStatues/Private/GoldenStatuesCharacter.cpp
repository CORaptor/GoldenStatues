// Copyright Epic Games, Inc. All Rights Reserved.

#include "GoldenStatuesCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/Interactable.h"
#include "Math/UnrealMathUtility.h"
#include "Components/HealthComponent.h"
#include "UI/QuestionWidget.h"

//////////////////////////////////////////////////////////////////////////
// AGoldenStatuesCharacter

AGoldenStatuesCharacter::AGoldenStatuesCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void AGoldenStatuesCharacter::AddScroll(TSubclassOf<class UQuestionWidget> QuestionClass)
{
	int Count = ScrollsPossessed.FindRef(QuestionClass);
	Count++;
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Current count: " + FString::FromInt(Count)));
	ScrollsPossessed.Add(QuestionClass, Count);
}

bool AGoldenStatuesCharacter::RemoveScroll(TSubclassOf<class UQuestionWidget> QuestionClass)
{
	int Count = ScrollsPossessed.FindRef(QuestionClass);
	if (Count <= 0) return false;

	Count--;
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Current count: " + FString::FromInt(Count)));
	ScrollsPossessed.Add(QuestionClass, Count);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGoldenStatuesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGoldenStatuesCharacter::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoldenStatuesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoldenStatuesCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGoldenStatuesCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGoldenStatuesCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGoldenStatuesCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGoldenStatuesCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGoldenStatuesCharacter::OnResetVR);
}


void AGoldenStatuesCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGoldenStatuesCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGoldenStatuesCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AGoldenStatuesCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	if (Rate != 0 && !bLockRotation)
	{
		bLockRotation = true; 
		AddControllerYawInput(FMath::Sign(Rate) * 90);

		FTimerDelegate TimerCallback;

		TimerCallback.BindLambda([&]
		{
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Green, "Unlocked");
			SetLockRotation(false);
		});

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 1.0f, false);
	}
}

void AGoldenStatuesCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGoldenStatuesCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bLockMovement)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (!EvaluateMovement(Direction, FMath::Sign(Value)))
		{
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, "Can't move in that direction");
			return;
		}

		bLockMovement = true;
		
		
		MoveForwardEvent(Value);
		
		// Free movement lock
		FTimerDelegate TimerCallback;

		TimerCallback.BindLambda([&]
		{
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Magenta, "Movement unlocked");
			SetLockMovement(false);
		});

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, MovementInterval, false);
	}
}

void AGoldenStatuesCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !bLockMovement)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); 
		
		if (!EvaluateMovement(Direction, FMath::Sign(Value)))
		{
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, "Can't move in that direction");
			return;
		}

		bLockMovement = true;
		
		MoveRightEvent(Value);

		FTimerDelegate TimerCallback;

		TimerCallback.BindLambda([&]
		{
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Magenta, "Movement unlocked");
			SetLockMovement(false);
		});

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, MovementInterval, false);
	}
}

void AGoldenStatuesCharacter::Interact()
{
	// check if the interaction target inmplements Interactable interface
	if (!UKismetSystemLibrary::DoesImplementInterface(InteractionTarget, UInteractable::StaticClass())) return;

	IInteractable::Execute_Interact(InteractionTarget);
}
