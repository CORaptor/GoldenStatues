// Fill out your copyright notice in the Description page of Project Settings.


#include "Statue.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GoldenStatuesCharacter.h"

// Sets default values
AStatue::AStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	InteractionRange = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRange"));
	InteractionRange->SetupAttachment(RootComponent);

}

AStatue * AStatue::FindStatue(FText SearcbedStatueName)
{
	TArray<AActor*> Statues;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStatue::StaticClass(), Statues);
	
	return nullptr;
}

// Called when the game starts or when spawned
void AStatue::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AGoldenStatuesCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	InteractionRange->OnComponentBeginOverlap.AddDynamic(this, &AStatue::OnEnterInteractionRange);
	InteractionRange->OnComponentEndOverlap.AddDynamic(this, &AStatue::OnLeaveInteractionRange);
}

void AStatue::OnEnterInteractionRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("Entered interaction range"));
	
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->SetInteractionTarget(this);
	}
}

void AStatue::OnLeaveInteractionRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Left interaction range"));

	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->SetInteractionTarget(nullptr);
	}
}

// Called every frame
void AStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

