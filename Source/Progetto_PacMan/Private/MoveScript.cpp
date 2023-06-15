// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveScript.h"


void IMoveScript::setLocalMap(TArray<FString> lm, FVector2D lms)
{
	localMap = lm;
	localMapSize = lms;
}

FVector2D IMoveScript::RelativePosition(FVector pos)
{
	return FVector2D(FMath::DivideAndRoundNearest((int32) pos.X, 100), FMath::DivideAndRoundNearest((int32) pos.Y, 100));
}

void IMoveScript::UpdatePosition() {}

void IMoveScript::DefinePosAround(int32 mode) {

	posAround = TArray<bool>();
	for (int32 i = 0; i < 4; i++)
		posAround.Add(false);
	UpdatePosAround(mode);

}

void IMoveScript::UpdatePosAround(int32 mode) {

	posAround[0] = IMapInterface::isValidPosition(Position + FVector2D(1, 0), localMapSize) && IMapInterface::isWalkable(localMap[Position.Y][Position.X + 1], mode);
	posAround[1] = IMapInterface::isValidPosition(Position + FVector2D(-1, 0), localMapSize) && IMapInterface::isWalkable(localMap[Position.Y][Position.X - 1], mode);
	posAround[2] = IMapInterface::isValidPosition(Position + FVector2D(0, 1), localMapSize) && IMapInterface::isWalkable(localMap[Position.Y + 1][Position.X], mode);
	posAround[3] = IMapInterface::isValidPosition(Position + FVector2D(0, -1), localMapSize) && IMapInterface::isWalkable(localMap[Position.Y - 1][Position.X], mode);

}
 
void IMoveScript::setPercentual(float perc) {

	Percentual = perc;

}

void IMoveScript::TeleportBehavior()
{
	
	// Exit the map by X's coodinate
	if (currentVelocity.X != 0 && (Position.X == 0 || Position.X == localMapSize.X - 1))
		posAround[currentVelocity.X > 0 ? 0 : 1] = true;

	// Exit the map by Y's coodinate
	if (currentVelocity.Y != 0 && (Position.Y == 0 || Position.Y == localMapSize.Y - 1))
		posAround[currentVelocity.Y > 0 ? 2 : 3] = true;
	
	// If he's outside the map by X's coordinate
	if (Position.X < 0 || Position.X >= localMapSize.X) {
		Position.X = FMath::Clamp(Position.X + (currentVelocity.X > 0 ? -1 : 1) * localMapSize.X, -1, localMapSize.X);
		Cast<AActor>(this)->SetActorLocation(FVector(Position.X * 100, Position.Y * 100, 0), false, nullptr, ETeleportType::TeleportPhysics);
		UpdatePosAround(1);
	}

	// If he's outside the map by Y's coordinate
	if (Position.Y < 0 || Position.Y >= localMapSize.Y) {
		Position.Y = FMath::Clamp(Position.Y + (currentVelocity.Y > 0 ? -1 : 1) * localMapSize.Y, -1, localMapSize.Y);
		Cast<AActor>(this)->SetActorLocation(FVector(Position.X * 100, Position.Y * 100, 0), false, nullptr, ETeleportType::TeleportPhysics);
		UpdatePosAround(1);
	}

}

FVector2D IMoveScript::getDirection()
{
	FVector2D absVelocity = FMath::Abs(currentVelocity);
	return FVector2D(absVelocity.X < 0.2 ? currentVelocity.X : (currentVelocity.X / absVelocity.X), absVelocity.Y < 0.2 ? currentVelocity.Y : (currentVelocity.Y / absVelocity.Y));
}
