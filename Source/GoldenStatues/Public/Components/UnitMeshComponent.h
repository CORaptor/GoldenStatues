// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "Interfaces/Interactable.h"

#include "UnitMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Party), meta = (BlueprintSpawnableComponent))
class GOLDENSTATUES_API UUnitMeshComponent : public USkeletalMeshComponent, public IInteractable
{
	GENERATED_BODY()
	
};
