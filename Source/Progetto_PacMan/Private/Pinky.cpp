// Fill out your copyright notice in the Description page of Project Settings.


#include "Pinky.h"

APinky::APinky()
{

	PrimaryActorTick.bCanEverTick = true;

}

// Game starts
void APinky::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* _Skin1 = Cast<UStaticMeshComponent>(this->GetDefaultSubobjectByName("Skin1"));
	UStaticMeshComponent* _Skin2 = Cast<UStaticMeshComponent>(this->GetDefaultSubobjectByName("Skin2"));
	_defaultColor = { UMaterialInstanceDynamic::Create(_Skin1->GetMaterial(0), _Skin1), UMaterialInstanceDynamic::Create(_Skin2->GetMaterial(0), _Skin2) };

	BeginPlayFunction();
	
}

void APinky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickFunction(DeltaTime);

}

void APinky::UpdateTargetChase()
{

	currentTarget = getPacManPos() + 4 * DatabasePtr->PacManDirection;

}

TArray<UMaterialInstanceDynamic*> APinky::getDefaultColor()
{
	return _defaultColor;
}
