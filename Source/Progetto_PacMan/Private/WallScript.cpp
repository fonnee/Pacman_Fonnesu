// Fill out your copyright notice in the Description page of Project Settings.


#include "WallScript.h"

AWallScript::AWallScript()
{

	PrimaryActorTick.bCanEverTick = false;

}

// Game starts
void AWallScript::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWallScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWallScript::ChangeRotation(FVector rotation) {
	
	SetActorRotation(FQuat::MakeFromEuler(rotation), ETeleportType::TeleportPhysics);
	
}