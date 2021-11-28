// Copyright Epic Games, Inc. All Rights Reserved.

#include "WordSearchGameMode.h"
#include "WordSearchPlayerController.h"
#include "WordSearchPawn.h"

AWordSearchGameMode::AWordSearchGameMode()
{
	// no pawn by default
	DefaultPawnClass = AWordSearchPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AWordSearchPlayerController::StaticClass();
}
