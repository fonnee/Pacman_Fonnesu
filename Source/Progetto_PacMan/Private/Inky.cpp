// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"

AInky::AInky()
{

	PrimaryActorTick.bCanEverTick = true;

}

// Game starts
void AInky::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* _Skin1 = Cast<UStaticMeshComponent>(this->GetDefaultSubobjectByName("Skin1"));
	UStaticMeshComponent* _Skin2 = Cast<UStaticMeshComponent>(this->GetDefaultSubobjectByName("Skin2"));
	_defaultColor = { UMaterialInstanceDynamic::Create(_Skin1->GetMaterial(0), _Skin1), UMaterialInstanceDynamic::Create(_Skin2->GetMaterial(0), _Skin2) };

	BeginPlayFunction();

}

void AInky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickFunction(DeltaTime);

}

void AInky::UpdateTargetChase()
{

	// ( Inky Target + Blinky Position) : 2 = Pacman Position + 2 Pacman Direction
	currentTarget = 2 * (getPacManPos() + 2 * DatabasePtr->PacManDirection) - DatabasePtr->RedGhostPosition;

}

TArray<UMaterialInstanceDynamic*> AInky::getDefaultColor()
{
	return _defaultColor;
}
