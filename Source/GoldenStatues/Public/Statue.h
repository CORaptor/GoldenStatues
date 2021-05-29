// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/Interactable.h"

#include "Statue.generated.h"

UCLASS()
class GOLDENSTATUES_API AStatue : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatue();

	UFUNCTION(BlueprintPure, Category = "Statue")
	static AStatue* FindStatue(UObject* WorldContextObject, FText StatueName);

	UFUNCTION(BlueprintPure, Category = "Statue")
	static TArray<FText> GetAllStatuesNames(UObject* WorldContextObject);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits")
	bool bLiar;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Traits")
	FText StatueName;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnterInteractionRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeaveInteractionRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	class AGoldenStatuesCharacter* PlayerCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
