// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthPoints = MaxHealthPoints;
	
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	HealthPoints = FMath::Clamp(0.f, HealthPoints - DamageAmount, MaxHealthPoints);

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, *FString("Health left:" + FString::SanitizeFloat(HealthPoints)));

	if (HealthPoints <= 0)
	{
		Die();
	}
}

void UHealthComponent::Die_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("I just died in your dungeons tonight."));
	OnDie.Broadcast();
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

