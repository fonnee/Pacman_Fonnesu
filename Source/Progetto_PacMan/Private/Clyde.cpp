// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"

AClyde::AClyde()
{

	PrimaryActorTick.bCanEverTick = true;

}

// Game starts
void AClyde::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* _Skin1 = Cast<UStaticMeshComponent>(this->GetDefaultSubobjectByName("Skin1"));
	UStaticMeshComponent* _Skin2 = Cast<UStaticMeshComponent>(this->GetDefaultSubobjectByName("Skin2"));
	_defaultColor = { UMaterialInstanceDynamic::Create(_Skin1->GetMaterial(0), _Skin1), UMaterialInstanceDynamic::Create(_Skin2->GetMaterial(0), _Skin2) };

	BeginPlayFunction();

}

void AClyde::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickFunction(DeltaTime);

}

void AClyde::UpdateTargetChase()
{

	currentTarget = FVector2D::Distance(getPacManPos(), Position) < 8 ? DefaultTarget : getPacManPos();

}

TArray<UMaterialInstanceDynamic*> AClyde::getDefaultColor()
{
	return _defaultColor;
}
