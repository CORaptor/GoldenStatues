// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestionWidget.h"

#include "Components/Button.h"
//#include "..\..\Public\UI\QuestionWidget.h"

void UQuestionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	QuestionButton->OnClicked.AddDynamic(this, &UQuestionWidget::OnButtonClicked);
}

void UQuestionWidget::OnButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, TEXT("Question asked"));
	OnQuestionAsked.Broadcast();
}