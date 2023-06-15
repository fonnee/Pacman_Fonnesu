// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodScript.generated.h"

UCLASS()
class PROGETTO_PACMAN_API AFoodScript : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AFoodScript();

	void setPU(bool b);

	void PacManEat();


protected:
	
	// Game startsd
	virtual void BeginPlay() override;

	// PU?
	bool PU;

	UPROPERTY()
		UStaticMeshComponent* _food;

	FTimerHandle TimerHandle_Blink;

public:	
	
	virtual void Tick(float DeltaTime) override;

};