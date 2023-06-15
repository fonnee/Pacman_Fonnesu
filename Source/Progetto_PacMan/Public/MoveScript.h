// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Interface.h"
#include "MapInterface.h"
#include "DatabaseVariablesScript.h"
#include "MoveScript.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMoveScript : public UInterface
{
	GENERATED_BODY()
};

/*
 * 
 */
class PROGETTO_PACMAN_API IMoveScript
{
	GENERATED_BODY()

public:

	// Reference to DatabaseVariablesScript
	ADatabaseVariablesScript* DatabasePtr;

	FVector2D RelativePosition(FVector pos);

	void setLocalMap(TArray<FString> lm, FVector2D lms);

	// In some Arcade the Speed was sometimes 75.75757625 pixel/s and in others 88 pixels/s, or respectively 9.469 tiles/s and 11 tiles/s (1 tile = 8x8 pixels)
	// Here it is set as the average speed of the two: ~10.24 tiles/s (as others suggest, the right value you should use as reference is 10 tiles/s)
	const float Velocity100Perc = 10.24;

	// Range Value: % - [0, 100]
	float Percentual;

	// Local Map [Note: they are classes, so they will be passed as a reference and not as a value]
	FVector2D localMapSize;
	TArray<FString> localMap;

	FVector2D Position;

	// Helper Array to avoid needlessly calling isWalkable() and isValidPosition()
	TArray<bool> posAround;

	FVector2D currentVelocity;

	// Function that is executed every time the position relative to tiles change
	virtual void UpdatePosition();

	// Functions to Define and Update posAround Array
	void DefinePosAround(int32 mode);
	void UpdatePosAround(int32 mode);

	// Function to set Percentual
	void setPercentual(float perc);

	// Behavior Function
	void TeleportBehavior();

	FVector2D getDirection();

};
