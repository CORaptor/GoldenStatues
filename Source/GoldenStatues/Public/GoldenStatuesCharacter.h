// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GoldenStatuesCharacter.generated.h"

UCLASS(config=Game)
class AGoldenStatuesCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AGoldenStatuesCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	void SetInteractionTarget(AActor* NewTarget) { InteractionTarget = NewTarget; }
	AActor* GetInteractionTarget() { return InteractionTarget; }

	void SetLockRotation(bool bShouldLock) { bLockRotation = bShouldLock; }
	void SetLockMovement(bool bShouldLock) { bLockMovement = bShouldLock; }

	UFUNCTION(BlueprintImplementableEvent)
	void MoveForwardEvent(float Value);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveRightEvent(float Value);

	// Check if you can move in the given direction
	UFUNCTION(BlueprintImplementableEvent)
	bool EvaluateMovement(FVector DirectionVector, float Sign);

	UFUNCTION(BlueprintCallable)
	void AddScroll(TSubclassOf<class UQuestionWidget> QuestionClass);

	UFUNCTION(BlueprintCallable)
	bool RemoveScroll(TSubclassOf<class UQuestionWidget> QuestionClass);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* Health;

	UPROPERTY()
	AActor* InteractionTarget;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Movement")
	float GridSize = 400;

	// How much you have to wait between movements
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementInterval = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	TMap<TSubclassOf<class UQuestionWidget>, int> ScrollsPossessed;

	bool bLockRotation = false;
	bool bLockMovement = false;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Interact();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

