// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostInterface.h"

void IGhostInterface::BeginPlayFunction()
{

	thisAA = Cast<AActor>(this);
	thisIMV = Cast<IMoveScript>(this);

	alreadyEaten = false;
	ActiveModel = false;
	
	Skin1 = Cast<UStaticMeshComponent>(thisAA->GetDefaultSubobjectByName("Skin1"));
	Skin2 = Cast<UStaticMeshComponent>(thisAA->GetDefaultSubobjectByName("Skin2"));
	Mouth = Cast<UStaticMeshComponent>(thisAA->GetDefaultSubobjectByName("Mouth"));
	Eyes = Cast<UStaticMeshComponent>(thisAA->GetDefaultSubobjectByName("Eyes"));
	LeftEyeBall = Cast<UStaticMeshComponent>(thisAA->GetDefaultSubobjectByName("LeftEyeBall"));
	RightEyeBall = Cast<UStaticMeshComponent>(thisAA->GetDefaultSubobjectByName("RightEyeBall"));
	
	refreshColor();

	FTimerHandle TimerHandle;
	thisAA->GetWorldTimerManager().SetTimer(TimerHandle, [&]() {MovementAnimation();}, 0.2f, true);

	actualPercentual = -1;

}

void IGhostInterface::TickFunction(float DeltaTime)
{

	for (int32 i = 0; i < 2; i++)
		if (!DefaultColor[i]->IsValidLowLevel() || !BlueColor[i]->IsValidLowLevel())
			refreshColor();

	if (!thisIMV->DatabasePtr->canGhostsMove) {
		UpdateState(currentState);
		thisIMV->currentVelocity = FVector2D(0, 0);
		return;
	}

	FVector2D newPosition = FVector2D(FMath::DivideAndRoundNearest((int32)thisAA->GetActorLocation().X, 100), FMath::DivideAndRoundNearest((int32)thisAA->GetActorLocation().Y, 100));

	if (newPosition != thisIMV->Position) {
		thisIMV->Position = newPosition;
		UpdateTarget();
	}

	// Calibrate Position
	if (currentState != 5 && Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize) && !Cast<IMapInterface>(this)->isGhostHouse(thisIMV->localMap[thisIMV->Position.Y][thisIMV->Position.X])) {
		if (FMath::Abs(thisIMV->currentVelocity.X) < 0.2)
			thisAA->SetActorLocation(FVector(thisIMV->Position.X * 100, thisAA->GetActorLocation().Y, 0), false, nullptr, ETeleportType::TeleportPhysics);
		if (FMath::Abs(thisIMV->currentVelocity.Y) < 0.2)
			thisAA->SetActorLocation(FVector(thisAA->GetActorLocation().X, thisIMV->Position.Y * 100, 0), false, nullptr, ETeleportType::TeleportPhysics);
	}

	
	if (!thisIMV->currentVelocity.IsZero())
	{
		FVector NewLocation = thisAA->GetActorLocation() + (FVector(thisIMV->currentVelocity.X, thisIMV->currentVelocity.Y, 0) * DeltaTime);
		thisAA->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else if (thisIMV->DatabasePtr->canGhostsMove)
		UpdateTarget();

	// Check if pacman touching ghost
		if (!thisIMV->DatabasePtr->PacManLost && FMath::Abs(thisAA->GetActorLocation().X - thisIMV->DatabasePtr->PacManPosition.X) + FMath::Abs(thisAA->GetActorLocation().Y - thisIMV->DatabasePtr->PacManPosition.Y) < 100) {
		if (thisIMV->DatabasePtr->isPacManPU && currentState == 2 && !alreadyEaten) {
			thisIMV->DatabasePtr->GhostsEaten++;
			UpdateState(4);
			UpdateTarget();
		}
		else if (currentState < 2)
			thisIMV->DatabasePtr->PacManLost = true;
	}
	
		if (!thisIMV->DatabasePtr->isPacManPU)
		alreadyEaten = false;

}

void IGhostInterface::UpdateVelocity(bool notBackward)
{

	if (Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize) && thisIMV->localMap[thisIMV->Position.Y][thisIMV->Position.X] == 'T' ||
		!Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize))
		thisIMV->TeleportBehavior();

	// Obtain direction
	if (notBackward) {
		if (thisIMV->currentVelocity.X != 0)
			thisIMV->posAround[thisIMV->currentVelocity.X > 0 ? 1 : 0] = false;
		if (thisIMV->currentVelocity.Y != 0)
			thisIMV->posAround[thisIMV->currentVelocity.Y > 0 ? 3 : 2] = false;
	}

	thisIMV->currentVelocity = ChooseFunction(thisIMV->posAround) * thisIMV->Percentual * thisIMV->Velocity100Perc;

}

FVector2D IGhostInterface::getPacManPos()
{
	return FVector2D(thisIMV->DatabasePtr->PacManPosition.X, thisIMV->DatabasePtr->PacManPosition.Y) / 100;
}

float IGhostInterface::DistanceFunction(FVector2D pos)
{
	return FVector2D::Distance(pos, currentTarget);
}

FVector2D IGhostInterface::ChooseFunction(TArray<bool> validDirection)
{

	TArray<float> distances = { 0, 0, 0, 0 };
	TArray<FVector2D> result = { FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1) };

	// Distances
	for (int32 i = 0; i < 4; i++)
		distances[i] = DistanceFunction(Cast<IMoveScript>(this)->Position + result[i]);

	TArray<float> sortedDistances = TArray<float>(distances);
	sortedDistances.Sort();
	for (int32 i = 0; i < 4; i++)
		for (int32 j = 0; j < 4; j++)
			if (validDirection[j] && sortedDistances[i] == distances[j])
				return result[j];

	return FVector2D(0, 0);

}

void IGhostInterface::UpdateState(int32 state)
{

	currentState = state;

	// Update direction and velocity (ghost changes direction)
	UpdateVelocity(false);

	// Animations
	if (currentState == 2) {
		Skin1->SetMaterial(0, BlueColor[0]);
		Skin2->SetMaterial(0, BlueColor[1]);
		Mouth->SetVisibility(true);
	}
	else {
		Skin1->SetMaterial(0, DefaultColor[0]);
		Skin2->SetMaterial(0, DefaultColor[1]);
		Mouth->SetVisibility(false);
	}
	if (currentState == 4) {
		Skin1->SetVisibility(false);
		Skin2->SetVisibility(false);
	}
	else {
		Skin1->SetVisibility(true);
		Skin2->SetVisibility(true);
	}

}

void IGhostInterface::UpdateTarget()
{

	bool InsideHouse = currentState >= 3 || Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize) &&
											Cast<IMapInterface>(this)->isGhostHouse(thisIMV->localMap[thisIMV->Position.Y][thisIMV->Position.X]);
	thisIMV->UpdatePosAround(InsideHouse ? 2 : 1);

	TArray<int32> settings = thisIMV->DatabasePtr->Settings;

	switch (currentState) {

	case 0:

		if (thisIMV->DatabasePtr->DefaultGhostState != 0) {
			UpdateState(thisIMV->DatabasePtr->DefaultGhostState);
			UpdateTarget();
			return;
		}

		UpdateTargetChase();

		if (Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize) && thisIMV->localMap[thisIMV->Position.Y][thisIMV->Position.X] == 'T')
			thisIMV->Percentual = settings[6];
		
		else
			thisIMV->Percentual = actualPercentual == -1 ? settings[5] : actualPercentual;

		break;

	case 1:

		if (thisIMV->DatabasePtr->DefaultGhostState != 1) {
			UpdateState(thisIMV->DatabasePtr->DefaultGhostState);
			UpdateTarget();
			return;
		}

		currentTarget = DefaultTarget;

		if (Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize) && thisIMV->localMap[thisIMV->Position.Y][thisIMV->Position.X] == 'T')
			thisIMV->Percentual = settings[6];
		
		else
			thisIMV->Percentual = actualPercentual == -1 ? settings[5] : actualPercentual;

		break;

	case 2:

		alreadyEaten = false;

		currentTarget = FVector2D(FMath::RandRange(0, (int32)thisIMV->localMapSize.X - 1), FMath::RandRange(0, (int32)thisIMV->localMapSize.Y - 1));

		if (Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize))
			thisIMV->Percentual = settings[13];

		break;

	case 3:

		currentTarget = SpawnTarget;

		if (Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize))
			thisIMV->Percentual = 0;

		break;

	case 4:

		// If ghost is inside home
		if (HomeTarget == thisIMV->Position) {
			alreadyEaten = true;
			UpdateState(5);
			UpdateTarget();
			return;
		}

		currentTarget = HomeTarget;

		if (Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize))
			thisIMV->Percentual = settings[16];

		break;

	case 5:

		if (NoHomeTarget == thisIMV->Position) {
			
			currentState = thisIMV->DatabasePtr->DefaultGhostState;
			UpdateTarget();
			
			return;
		}

		currentTarget = NoHomeTarget;

		if (Cast<IMapInterface>(this)->isValidPosition(thisIMV->Position, thisIMV->localMapSize))
			thisIMV->Percentual = settings[6];

		break;

	default:

		UpdateState(thisIMV->DatabasePtr->DefaultGhostState);

		break;

	}

	UpdateVelocity(true);

	// Animations
	Eyes->SetRelativeLocation(FVector(10 * thisIMV->getDirection().X, 10 * thisIMV->getDirection().Y + (currentState == 2 ? -10 : 0), 0), false, nullptr, ETeleportType::TeleportPhysics);
	LeftEyeBall->SetRelativeLocation(25 * FVector(thisIMV->getDirection().X, thisIMV->getDirection().Y, 0), false, nullptr, ETeleportType::TeleportPhysics);
	RightEyeBall->SetRelativeLocation(LeftEyeBall->GetRelativeLocation(), false, nullptr, ETeleportType::TeleportPhysics);

}

void IGhostInterface::UpdateTargetChase()
{
	currentTarget = getPacManPos();
}

void IGhostInterface::MovementAnimation()
{
	
	Skin1->SetRelativeLocation(FVector(0, 0, ActiveModel ? 0 : -2), false, nullptr, ETeleportType::TeleportPhysics);
	Skin2->SetRelativeLocation(FVector(0, 0, ActiveModel ? -2 : 0), false, nullptr, ETeleportType::TeleportPhysics);

	ActiveModel = !ActiveModel;
	
}

TArray<UMaterialInstanceDynamic*> IGhostInterface::getDefaultColor()
{
	return DefaultColor;
}

void IGhostInterface::refreshColor()
{
	DefaultColor = getDefaultColor();
	BlueColor = { UMaterialInstanceDynamic::Create(Skin1->GetMaterial(0), Skin1), UMaterialInstanceDynamic::Create(Skin2->GetMaterial(0), Skin2) };
	
	for (int32 i = 0; i < 2; i++)
		BlueColor[i]->SetVectorParameterValue("Color", FLinearColor(0.0, 0.0, 0.5, 1.0));
}
