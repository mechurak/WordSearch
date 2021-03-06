// Copyright Epic Games, Inc. All Rights Reserved.

#include "WordSearchPawn.h"
#include "WordSearchBlock.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AWordSearchPawn::AWordSearchPawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AWordSearchPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if (APlayerController* PC = Cast<APlayerController>(GetController()))
	// {
	// 	FVector Start, Dir, End;
	// 	PC->DeprojectMousePositionToWorld(Start, Dir);
	// 	End = Start + (Dir * 8000.0f);
	// 	TraceForBlock(Start, End, false);
	// }
}

void AWordSearchPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent->BindAction("ResetVR", EInputEvent::IE_Pressed, this, &AWordSearchPawn::OnResetVR);
	// PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &AWordSearchPawn::TriggerClick);
}

void AWordSearchPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void AWordSearchPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AWordSearchPawn::TriggerClick()
{
	if (CurrentBlockFocus)
	{
		CurrentBlockFocus->HandleClicked();
	}
}

void AWordSearchPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		AWordSearchBlock* HitBlock = Cast<AWordSearchBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}
}