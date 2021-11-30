// Copyright Epic Games, Inc. All Rights Reserved.

#include "WordSearchPlayerController.h"

AWordSearchPlayerController::AWordSearchPlayerController()
{
	// bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableTouchOverEvents = bEnableMouseOverEvents = true;
	// DefaultMouseCursor = EMouseCursor::Crosshairs;
}
