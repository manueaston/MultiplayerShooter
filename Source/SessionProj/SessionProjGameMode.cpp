// Copyright Epic Games, Inc. All Rights Reserved.

#include "SessionProjGameMode.h"
#include "SessionProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASessionProjGameMode::ASessionProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
