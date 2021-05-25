// Fill out your copyright notice in the Description page of Project Settings.


#include "Scroll.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UI/QuestionWidget.h"

// Sets default values
AScroll::AScroll()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	InteractionRange = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Range"));
	InteractionRange->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AScroll::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScroll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

