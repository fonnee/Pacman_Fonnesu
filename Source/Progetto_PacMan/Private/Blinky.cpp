// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

ABlinky::ABlinky()
{

	PrimaryActorTick.bCanEverTick = true;

}

// Game starts
void ABlinky::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* _Skin1 = Cast<UStaticMeshComponent>(this->GetDefaultSubobjectByName("Skin1"));
	UStaticMeshComponent* _Skin2 = Cast<UStaticMeshComponent>(this->GetDefaultSubobjectByName("Skin2"));
	_defaultColor = { UMaterialInstanceDynamic::Create(_Skin1->GetMaterial(0), _Skin1), UMaterialInstanceDynamic::Create(_Skin2->GetMaterial(0), _Skin2) };

	BeginPlayFunction();

}

void ABlinky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickFunction(DeltaTime);

	DatabasePtr->RedGhostPosition = Position;

	if (DatabasePtr->LeftOverFood <= DatabasePtr->Settings[9])
		actualPercentual = DatabasePtr->Settings[10];

	else if (DatabasePtr->LeftOverFood <= DatabasePtr->Settings[7])
		actualPercentual = DatabasePtr->Settings[8];

	else actualPercentual = DatabasePtr->Settings[5];

}

TArray<UMaterialInstanceDynamic*> ABlinky::getDefaultColor()
{
	return _defaultColor;
}
