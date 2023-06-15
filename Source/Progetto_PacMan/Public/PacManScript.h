// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Quat.h"
#include "MoveScript.h"
#include "FoodScript.h"
#include "PacManScript.generated.h"

UCLASS()
class PROGETTO_PACMAN_API APacManScript : public APawn, public IMoveScript
{
	GENERATED_BODY()

public:
	
	APacManScript();

protected:
	
	// Game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		TArray<FFoodPointer> FoodArray;

	UPROPERTY()
		UStaticMeshComponent* BodyComponent;

	UPROPERTY()
		TArray<UStaticMeshComponent*> Parts;

	float currentRotation;
	float rotationAmplitude;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void UpdatePosition() override;

	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void setFoodArray(TArray<FFoodPointer> foodArray);

	AFoodScript* SearchAndEatFood(FVector2D pos);

	bool StartedGame;

};
