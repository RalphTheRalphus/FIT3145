// Copyright Epic Games, Inc. All Rights Reserved.

#include "Grappling_HookGameMode.h"
#include "Grappling_HookCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGrappling_HookGameMode::AGrappling_HookGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
