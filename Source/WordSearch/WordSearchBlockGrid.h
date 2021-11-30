// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WordSearchBlock.h"
#include "GameFramework/Actor.h"
#include "WordSearchBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AWordSearchBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

public:
	AWordSearchBlockGrid();

	UFUNCTION()
	void SetStartBlock(int32 NewStartBlockPos);

	UFUNCTION()
	void SetCurrentBlock(int32 NewCurrentBlockPos);

	UFUNCTION()
	void Evaluate();


	/** How many blocks have been clicked */
	int32 Score;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

public:

	/** Handle the block being clicked */
	void AddScore();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }

private:
	int32 getDirection();

protected:
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	TArray<AWordSearchBlock*> Blocks;
	
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32 StartBlockPos;

	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32 CurrentBlockPos;
};



