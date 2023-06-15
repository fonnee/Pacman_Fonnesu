// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/Quat.h"
#include "Math/Vector2D.h"
#include "MoveScript.h"
#include "GhostInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGhostInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROGETTO_PACMAN_API IGhostInterface
{
	GENERATED_BODY()

public:

	// Spawn Position/Target
	FVector2D SpawnTarget;

	// Ghosts' Home Position/Target
	FVector2D HomeTarget;

	// Outside Ghosts' Home Position/Target
	FVector2D NoHomeTarget;

	// Default Target
	FVector2D DefaultTarget;

	// What is targeting now
	FVector2D currentTarget;

	// Indicate what is Ghost's State (0:Chase, 1:Scatter, 2:Frightened, 3:WaitInHouse, 4:GoToHouse, 5:ExitFromHouse)
	int32 currentState;

	// Function that Update Velocity (and Position)
	void UpdateVelocity(bool notBackward);

	// Get PacMan Position
	FVector2D getPacManPos();

	// Calculate Distance from Target
	float DistanceFunction(FVector2D pos);

	// Choose Direction Function (closest target => smaller result), return Direction
	// Note: validDirection define if (1, 0), (-1, 0), (0, 1) and (0, -1) are valid Direction
	FVector2D ChooseFunction(TArray<bool> validDirection);

	// Function that Update Ghost's State (Used for praticity)
	virtual void UpdateState(int32 state);
	
	// Function that Update Target Position
	virtual void UpdateTarget();

	// Function that is call when Updating Target Position in Chase Mode (default: it tracks PacMan)
	virtual void UpdateTargetChase();

	// Animation Functions
	void MovementAnimation();
	virtual TArray<UMaterialInstanceDynamic*> getDefaultColor();
	void refreshColor();

protected:
	// These Functions have to be placed inside BeginPlay and Tick respectively
	void BeginPlayFunction();
	void TickFunction(float DeltaTime);

	// Helper Variable used to keep track if Ghost can eat PacMan
	bool alreadyEaten;

	// Helper Variable (Used only by Red)
	int32 actualPercentual;

	// Helper Reference
	AActor* thisAA;
	IMoveScript* thisIMV;

	// Variables used for Animation
	TArray<UMaterialInstanceDynamic*> DefaultColor;
	TArray<UMaterialInstanceDynamic*> BlueColor;
	UStaticMeshComponent* Skin1;
	UStaticMeshComponent* Skin2;
	UStaticMeshComponent* Eyes;
	UStaticMeshComponent* Mouth;
	UStaticMeshComponent* LeftEyeBall;
	UStaticMeshComponent* RightEyeBall;
	bool ActiveModel;

};
