// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDie);


UCLASS( ClassGroup=(Health), meta=(BlueprintSpawnableComponent) )
class GOLDENSTATUES_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void Die();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditANywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealthPoints = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float HealthPoints;

	UPROPERTY(BlueprintAssignable, Category = "Health|Event")
	FDie OnDie;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
