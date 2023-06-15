// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManScript.h"
#include "GameFramework/Actor.h"


APacManScript::APacManScript()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	Percentual = 80;

	StartedGame = true;

}

// Game starts
void APacManScript::BeginPlay()
{
	Super::BeginPlay();

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	BodyComponent = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName("Body"));
	Parts = { Cast<UStaticMeshComponent>(GetDefaultSubobjectByName("Part1")), Cast<UStaticMeshComponent>(GetDefaultSubobjectByName("Part2")) };
	currentRotation = 0;
	rotationAmplitude = 12;

	DefinePosAround(1);
	
}

void APacManScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!DatabasePtr->canPacManMove) {
		currentVelocity = FVector2D(0, 0);
		return;
	}

	else if (StartedGame) {
		StartedGame = false;
		currentVelocity.X = Percentual * Velocity100Perc;
		BodyComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 0)), false, nullptr, ETeleportType::TeleportPhysics);
	}

	FVector2D newPosition = FVector2D(FMath::DivideAndRoundNearest((int32)GetActorLocation().X, 100), FMath::DivideAndRoundNearest((int32)GetActorLocation().Y, 100));

	if (newPosition != Position) {
		Position = newPosition;
		UpdatePosition();
	}

	
	if (currentVelocity.X != 0 && !posAround[currentVelocity.X > 0 ? 0 : 1])
		currentVelocity.X = 0;
	
	if (currentVelocity.Y != 0 && !posAround[currentVelocity.Y > 0 ? 2 : 3])
		currentVelocity.Y = 0;

	// Calibrate position
	if (FMath::Abs(currentVelocity.X) < 0.2) {
		
		if (Position.X < 0) {
			posAround[0] = true;
			Move_XAxis(1);
		}
		
		else if (Position.X >= localMapSize.X) {
			posAround[1] = true;
			Move_XAxis(-1);
		}
		
		SetActorLocation(FVector(Position.X * 100, GetActorLocation().Y, 0), false, nullptr, ETeleportType::TeleportPhysics);
	}
	
	if (FMath::Abs(currentVelocity.Y) < 0.2) {
	
		if (Position.Y < 0) {
			posAround[2] = true;
			Move_YAxis(1);
		}
		
		else if (Position.Y >= localMapSize.Y) {
			posAround[3] = true;
			Move_YAxis(-1);
		}
		
		SetActorLocation(FVector(GetActorLocation().X, Position.Y * 100, 0), false, nullptr, ETeleportType::TeleportPhysics);
	}

	if (!currentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (FVector(currentVelocity.X, currentVelocity.Y, 0) * DeltaTime);
		SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

		// Animations
		currentRotation += rotationAmplitude;
		
		if (currentRotation >= 60 || currentRotation <= 0)
			rotationAmplitude *= -1;
		
		FVector rotation = FVector(0, 0, FMath::Clamp(currentRotation, 0, 60));
		Parts[0]->SetRelativeRotation(FQuat::MakeFromEuler(rotation), false, nullptr, ETeleportType::TeleportPhysics);
		Parts[1]->SetRelativeRotation(FQuat::MakeFromEuler(-rotation), false, nullptr, ETeleportType::TeleportPhysics);

	}

	DatabasePtr->PacManPosition = GetActorLocation();
	DatabasePtr->PacManDirection = getDirection();

}

void APacManScript::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveX", this, &APacManScript::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &APacManScript::Move_YAxis);

}

void APacManScript::Move_XAxis(float AxisValue)
{

	if (FMath::Abs(GetActorLocation().Y - 100 * Position.Y) > 30 || !DatabasePtr->canPacManMove)
		return;

	if (AxisValue > 0 && posAround[0]) {
		currentVelocity.X = Percentual * Velocity100Perc;
		currentVelocity.Y = 0;
		BodyComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 0)), false, nullptr, ETeleportType::TeleportPhysics);
	}

	else if (AxisValue < 0 && posAround[1]) {
		currentVelocity.X = -Percentual * Velocity100Perc;
		currentVelocity.Y = 0;
		BodyComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 180)), false, nullptr, ETeleportType::TeleportPhysics);
	}

}

void APacManScript::Move_YAxis(float AxisValue)
{

	if (FMath::Abs(GetActorLocation().X - 100 * Position.X) > 30 || !DatabasePtr->canPacManMove)

		return;

	if (AxisValue > 0 && posAround[2]) {
		currentVelocity.Y = Percentual * Velocity100Perc;
		currentVelocity.X = 0;
		BodyComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 90)), false, nullptr, ETeleportType::TeleportPhysics);
	}

	else if (AxisValue < 0 && posAround[3]) {
		currentVelocity.Y = -Percentual * Velocity100Perc;
		currentVelocity.X = 0;
		BodyComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, -90)), false, nullptr, ETeleportType::TeleportPhysics);
	}

}

void APacManScript::setFoodArray(TArray<FFoodPointer> foodArray) {

	FoodArray = TArray<FFoodPointer>(foodArray);

}

AFoodScript* APacManScript::SearchAndEatFood(FVector2D pos) {

	for (int32 i = 0; i < FoodArray.Num(); i++)

		if (FoodArray[i].pos == pos) {
			AFoodScript* temp = FoodArray[i].ref;
			FoodArray.RemoveAt(i);
			DatabasePtr->LeftOverFood--;
			Percentual = DatabasePtr->Settings[DatabasePtr->isPacManPU ? 12 : 4];

			return temp;
		}

	return nullptr;

}

void APacManScript::UpdatePosition() {

	UpdatePosAround(1);

	Percentual = DatabasePtr->Settings[DatabasePtr->isPacManPU ? 11 : 3];

	if (localMap.Num() == 0)
		return;
	
	switch (IMapInterface::isValidPosition(Position, localMapSize) ? localMap[Position.Y][Position.X] : 'T') {
	
	// Food
	case 'o':
	{

		AFoodScript* food = SearchAndEatFood(Position);
		if (food != nullptr)
			food->PacManEat();

		break;

	}
	
	// PU
	case '@':
	{

		AFoodScript* food = SearchAndEatFood(Position);
		
		if (food != nullptr) {
			DatabasePtr->PU = true;
			food->PacManEat();
		}

		break;

	}
	
	// Teleport
	case 'T':

		TeleportBehavior();

		break;

	default:

		break;

	}

}