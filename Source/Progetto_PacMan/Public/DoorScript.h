// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorScript.generated.h"

UCLASS()
class PROGETTO_PACMAN_API ADoorScript : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ADoorScript();

	void ChangeRotation(FVector r);

protected:
	
	// Game starts
	virtual void BeginPlay() override;

	FVector newRotation = FVector(0, 0, 0);

public:	
	
	virtual void Tick(float DeltaTime) override;

};
