// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Actor.h"
#include "MyCamera.generated.h"

/**
 * 
 */
UCLASS()
class PROGETTO_PACMAN_API AMyCamera : public ACameraActor
{
	GENERATED_BODY()

public:

	void setPosition(FVector pos);
	void setRotation(FVector rot);
	void setPositionAndRotation(FVector pos, FVector rot);

};
