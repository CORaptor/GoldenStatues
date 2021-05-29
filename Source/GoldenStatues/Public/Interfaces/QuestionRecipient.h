// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestionRecipient.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestionRecipient : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOLDENSTATUES_API IQuestionRecipient
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Questions")
	bool Answer(bool FactualState);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Questions")
	void RefuseToAnswer();

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
