// Copyright Epic Games, Inc. All Rights Reserved.

#include "GoldenStatuesGameMode.h"
#include "GoldenStatuesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGoldenStatuesGameMode::AGoldenStatuesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
