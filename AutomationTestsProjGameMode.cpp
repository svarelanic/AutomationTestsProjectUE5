// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationTestsProjGameMode.h"
#include "AutomationTestsProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAutomationTestsProjGameMode::AAutomationTestsProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
