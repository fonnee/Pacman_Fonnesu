// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DatabaseVariablesScript.h"
#include "MapInterface.h"
#include "GhostInterface.h"
#include "MyCamera.h"
#include "PacManScript.h"
#include "Blinky.h"
#include "Inky.h"
#include "Pinky.h"
#include "Clyde.h"
#include "FoodScript.h"
#include "WallScript.h"
#include "DoorScript.h"
#include "MapScript.generated.h"


UCLASS()
class PROGETTO_PACMAN_API AMapScript : public AActor, public IMapInterface
{
	GENERATED_BODY()
	
public:	
	
	AMapScript();

	TArray<FString> Map;
	FVector2D MapSize;

	TArray<FFoodPointer> FoodArray;

protected:
	
	// Game starts
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		ADatabaseVariablesScript* DatabaseRef;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AMyCamera> CameraClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APacManScript> PacManClass;

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<AActor>> GhostClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AFoodScript> FoodClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> Empty_TileClass;

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<AWallScript>> WallClass;

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<AWallScript>> DoubleWallClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADoorScript> DoorClass;

	TArray<AActor*> Generate(TArray<FString> map, FVector2D size);

	void ReGenerate(TArray<FString> map, FVector2D size, TArray<AActor*> characters, bool restoreFood);

	bool callWalkable(int32 x, int32 y, int32 mode);
	bool callGhostHouse(int32 x, int32 y);

};


