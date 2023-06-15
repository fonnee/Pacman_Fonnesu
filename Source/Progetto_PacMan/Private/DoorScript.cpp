// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorScript.h"

ADoorScript::ADoorScript()
{

	PrimaryActorTick.bCanEverTick = false;

}

// Game starts
void ADoorScript::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FQuat::MakeFromEuler(newRotation), ETeleportType::TeleportPhysics);
	
}

void ADoorScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorScript::ChangeRotation(FVector r) {
	newRotation = r;
	SetActorRotation(FQuat::MakeFromEuler(newRotation), ETeleportType::TeleportPhysics);
}

