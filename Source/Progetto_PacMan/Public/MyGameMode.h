// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DatabaseVariablesScript.h"
#include "MapScript.h"
#include "MyGameMode.generated.h"


UCLASS()
class PROGETTO_PACMAN_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		ADatabaseVariablesScript* Database;

	UPROPERTY()
		AMapScript* Map;

	bool GameNotStarted;

	// Game scores
	int32 Score;
	int32 HighScore;
	
	FTimerHandle GlobalTimerHandle;
	int32 GameTime;

	UPROPERTY()
		TArray<AActor*> Characters;

	// Ghosts' mode
	FTimerHandle TimerHandle_GhostMode;
	FTimerHandle TimerHandle_GhostFrightened;
	
	bool hasChangedMode;

	int32 StateCounter;
	int32 localPoint;	
	int32 localTotalFood;
	int32 localLeftOverFood;
	int32 localGhostsEaten;

	void UseFile(bool ReadMode);

	UFUNCTION(BlueprintCallable)
		int32 getScore();

	UFUNCTION(BlueprintCallable)
		int32 getHighScore();

	UFUNCTION(BlueprintCallable)
		int32 getPacManLives();
	
};

