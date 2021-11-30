// Copyright Epic Games, Inc. All Rights Reserved.

#include "WordSearchBlock.h"
#include "WordSearchBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

AWordSearchBlock::AWordSearchBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);

	// BlockMesh->OnClicked.AddDynamic(this, &AWordSearchBlock::BlockClicked);	
	// BlockMesh->OnInputTouchBegin.AddDynamic(this, &AWordSearchBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
}

void AWordSearchBlock::BeginPlay()
{
	Super::BeginPlay();
	
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AWordSearchBlock::OnFingerPressedBlock);
	BlockMesh->OnInputTouchEnd.AddDynamic(this, &AWordSearchBlock::OnFingerEndedBlock);
	BlockMesh->OnInputTouchEnter.AddDynamic(this, &AWordSearchBlock::OnFingerEnteredBlock);
	BlockMesh->OnInputTouchLeave.AddDynamic(this, &AWordSearchBlock::OnFingerLeavedBlock);
}

void AWordSearchBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFingerPressedBlock"));	
	if (!bIsActive)
	{
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, OrangeMaterial);

		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->SetStartBlock(GridAddress);
		}
	}
}

void AWordSearchBlock::OnFingerEndedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFingerEndedBlock"));
	if (OwningGrid != nullptr)
	{
		OwningGrid->Evaluate();
	}	
}

void AWordSearchBlock::OnFingerEnteredBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFingerEnteredBlock"));
	if (OwningGrid != nullptr)
	{
		OwningGrid->SetCurrentBlock(GridAddress);
	}	
}

void AWordSearchBlock::OnFingerLeavedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFingerLeavedBlock"));	
}

void AWordSearchBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}

void AWordSearchBlock::HandleClicked()
{
	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, OrangeMaterial);

		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->AddScore();
		}
	}
}

void AWordSearchBlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void AWordSearchBlock::ResetBlock()
{
	bIsActive = false;
	BlockMesh->SetMaterial(0, BlueMaterial);
}

void AWordSearchBlock::Select()
{
	bIsActive = true;
	BlockMesh->SetMaterial(0, OrangeMaterial);
}
