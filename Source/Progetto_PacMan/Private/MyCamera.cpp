// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCamera.h"

void AMyCamera::setPosition(FVector pos) {

	SetActorLocation(pos, false, nullptr, ETeleportType::TeleportPhysics);

}

void AMyCamera::setRotation(FVector rot) {

	SetActorRotation(FQuat::MakeFromEuler(rot), ETeleportType::TeleportPhysics);

}

void AMyCamera::setPositionAndRotation(FVector pos, FVector rot) {

	setPosition(pos);
	setRotation(rot);

}