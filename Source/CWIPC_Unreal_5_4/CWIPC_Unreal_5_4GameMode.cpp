// Copyright Epic Games, Inc. All Rights Reserved.

#include "CWIPC_Unreal_5_4GameMode.h"
#include "CWIPC_Unreal_5_4Character.h"
#include "UObject/ConstructorHelpers.h"

ACWIPC_Unreal_5_4GameMode::ACWIPC_Unreal_5_4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
