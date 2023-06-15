// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DatabaseVariablesScript.generated.h"

UCLASS()
class PROGETTO_PACMAN_API ADatabaseVariablesScript : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ADatabaseVariablesScript();

	FVector2D MapSize;
	TArray<FString> Map;
	TArray<int32> Settings;
	TArray<float> GhostModeTimers;
	TArray<float> GhostFree;

	bool LoadAll();
	bool canPacManMove;
	bool canGhostsMove;
	int32 DefaultGhostState;
	bool isPacManPU;

	FVector PacManPosition;
	FVector2D PacManDirection;
	int32 LivesLeft;
	int32 LeftOverFood;
	bool PU;

	FVector2D RedGhostPosition;
	bool PacManLost;
	int32 GhostsEaten;

	UFUNCTION(BlueprintCallable)
		int32 GetIndexSprite();

protected:
	
	// Game starts
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};