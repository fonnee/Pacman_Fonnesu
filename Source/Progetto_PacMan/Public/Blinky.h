// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveScript.h"
#include "GhostInterface.h"
#include "Blinky.generated.h"

UCLASS()
class PROGETTO_PACMAN_API ABlinky : public AActor, public IGhostInterface, public IMoveScript
{
	GENERATED_BODY()

public:
	
	ABlinky();

protected:
	
	// Game starts
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		TArray<UMaterialInstanceDynamic*> _defaultColor;

	virtual TArray<UMaterialInstanceDynamic*> getDefaultColor() override;

};
