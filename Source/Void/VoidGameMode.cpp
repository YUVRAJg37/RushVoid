// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoidGameMode.h"
#include "VoidCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVoidGameMode::AVoidGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
