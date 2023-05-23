// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_ReloadGameMode.h"
#include "Project_ReloadCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_ReloadGameMode::AProject_ReloadGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
