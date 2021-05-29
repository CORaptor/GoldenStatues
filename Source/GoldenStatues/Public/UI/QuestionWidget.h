// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAskQuestion);

/**
 * 
 */
UCLASS()
class GOLDENSTATUES_API UQuestionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FAskQuestion OnQuestionAsked;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* QuestionButton;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Text")
	FText QuestionText;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();
};
