// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FoodScript.h"
#include "MapInterface.generated.h"

USTRUCT()
struct FFoodPointer
{
	GENERATED_BODY()

public:

	FFoodPointer() {
		ref = nullptr;
		pos = FVector2D(0, 0);
	}

	FFoodPointer(AFoodScript* _ref, FVector2D _pos) {
		ref = _ref;
		pos = _pos;
	}

	AFoodScript* ref;
	FVector2D pos;

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMapInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROGETTO_PACMAN_API IMapInterface
{
	GENERATED_BODY()

public:

	static bool isValidPosition(FVector2D position, FVector2D MaxLimit);
	static bool isValidPosition(int32 x, int32 y, FVector2D MaxLimit);
	static bool isWalkable(char tile, int32 mode); // Mode = 0:Script Only, 1:PacMan, 2:Ghosts
	static bool isGhostHouse(char tile);

};
