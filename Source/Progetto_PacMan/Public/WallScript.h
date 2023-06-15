// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallScript.generated.h"

UCLASS()
class PROGETTO_PACMAN_API AWallScript : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AWallScript();

protected:
	// Game starts
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	
	void ChangeRotation(FVector rotation);

};

