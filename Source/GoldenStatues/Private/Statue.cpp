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

AStatue * AStatue::FindStatue(UObject* WorldContextObject, FText SearcbedStatueName)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, AStatue::StaticClass(), Actors);

	for(AActor* Actor : Actors)
	{
		AStatue* Statue = Cast<AStatue>(Actor);
		if (Statue == nullptr) continue;

		if (Statue->StatueName.EqualTo(SearcbedStatueName))
			return Statue;
	}
	
	return nullptr;
}

TArray<FText> AStatue::GetAllStatuesNames(UObject* WorldContextObject)
{
	TArray<FText> Names = TArray<FText>();
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, AStatue::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		AStatue* Statue = Cast<AStatue>(Actor);
		if (Statue == nullptr) continue;

		Names.Add(Statue->StatueName);
	}

	return Names;
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

