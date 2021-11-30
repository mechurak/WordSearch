// Copyright Epic Games, Inc. All Rights Reserved.

#include "WordSearchBlockGrid.h"
#include "WordSearchBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AWordSearchBlockGrid::AWordSearchBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Size = 3;
	BlockSpacing = 300.f;
}

void AWordSearchBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Loop to spawn each block
	for (int32 BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex % Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AWordSearchBlock* NewBlock = GetWorld()->SpawnActor<AWordSearchBlock>(BlockLocation, FRotator(0, 0, 0));		

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{			
			NewBlock->OwningGrid = this;
			NewBlock->SetGridAddress(BlockIndex);
			Blocks.Add(NewBlock);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%d blocks are spawned!"), Blocks.Num());
}


void AWordSearchBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}


void AWordSearchBlockGrid::SetStartBlock(int32 NewStartBlockPos)
{
	StartBlockPos = NewStartBlockPos;
	UE_LOG(LogTemp, Warning, TEXT("SetStartBlock : %d"), NewStartBlockPos);
}

void AWordSearchBlockGrid::SetCurrentBlock(int32 NewCurrentBlockPos)
{
	CurrentBlockPos = NewCurrentBlockPos;
	UE_LOG(LogTemp, Warning, TEXT("SetCurrentBlock : (%d, %d)"), StartBlockPos, CurrentBlockPos);

	const int32 matchDirection = getDirection();
	UE_LOG(LogTemp, Warning, TEXT("matchDirection: %d"), matchDirection);
	if (matchDirection != 0)
	{
		for (auto* block : Blocks)
		{
			block->ResetBlock();
		}

		int32 temp = StartBlockPos;
		while (temp != CurrentBlockPos)
		{
			Blocks[temp]->Select();
			temp += matchDirection;
		}
		UE_LOG(LogTemp, Warning, TEXT("TODO: Change Color (%d)"), temp);
		Blocks[temp]->Select();
	}
}

void AWordSearchBlockGrid::Evaluate()
{
	UE_LOG(LogTemp, Warning, TEXT("Evaluate : (%d, %d)"), StartBlockPos, CurrentBlockPos);

	// const int32 matchDirection = getDirection();
	// UE_LOG(LogTemp, Warning, TEXT("matchDirection: %d"), matchDirection);
	// if (matchDirection != 0)
	// {
	// 	int32 temp = StartBlockPos;
	// 	while (temp != CurrentBlockPos)
	// 	{
	// 		Blocks[temp]->Select();				
	// 		temp += matchDirection;
	// 	}
	// 	Blocks[temp]->Select();
	// }

	// Reset
	StartBlockPos = -1;
	CurrentBlockPos = -1;

	for (auto* block : Blocks)
	{
		block->ResetBlock();
	}
}

int32 AWordSearchBlockGrid::getDirection()
{
	int32 startRow = StartBlockPos / Size;
	int32 curRow = CurrentBlockPos / Size;
	if (startRow == curRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("startRow: %d, curRow: %d"), startRow, curRow);
		return CurrentBlockPos > StartBlockPos ? 1 : -1;
	}

	int32 startCol = StartBlockPos % Size;
	int32 curCol = CurrentBlockPos % Size;
	if (startCol == curCol)
	{
		UE_LOG(LogTemp, Warning, TEXT("startCol: %d, curCol: %d"), startCol, curCol);
		return CurrentBlockPos > StartBlockPos ? Size : -Size;
	}

	int32 dx = curRow - startRow;
	int32 dy = curCol - startCol;

	if (abs(dx) == abs(dy))
	{
		if (dx * dy > 0)
		{
			int32 largeCandidate = Size + 1; // up + right
			UE_LOG(LogTemp, Warning, TEXT("isDiagonal! up right"));
			return CurrentBlockPos > StartBlockPos ? largeCandidate : -largeCandidate;
		}
		else
		{
			int32 smallCandidate = Size - 1; // down + right 
			UE_LOG(LogTemp, Warning, TEXT("isDiagonal! down right"));
			return CurrentBlockPos > StartBlockPos ? smallCandidate : -smallCandidate;
		}
	}

	return 0; // ignore
}

#undef LOCTEXT_NAMESPACE
