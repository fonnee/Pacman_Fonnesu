// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodScript.h"

AFoodScript::AFoodScript()
{

	PrimaryActorTick.bCanEverTick = false;
	PU = false;

}

// Game starts
void AFoodScript::BeginPlay()
{
	Super::BeginPlay();

	_food = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName("Food"));

}

void AFoodScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodScript::setPU(bool b) {

	PU = b;

	if (PU) {
		
		// Change food 
		_food->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));
		
		// Activation "Blink" Animation
		GetWorldTimerManager().SetTimer(TimerHandle_Blink, [&]() { _food->SetVisibility(!_food->IsVisible()); }, 0.15f, true);
	}
	else {
		
		// Change food
		_food->SetRelativeScale3D(FVector(0.3, 0.3, 1.0));

		// Disable "Blink" Animation
		if (GetWorldTimerManager().IsTimerActive(TimerHandle_Blink))
			GetWorldTimerManager().ClearTimer(TimerHandle_Blink);
		_food->SetVisibility(true);
	}

}

void AFoodScript::PacManEat()
{
	Destroy();
}
