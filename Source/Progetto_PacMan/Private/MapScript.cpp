// Fill out your copyright notice in the Description page of Project Settings.


#include "MapScript.h"
#include "Math/Quat.h"
#include "Kismet/GameplayStatics.h"

AMapScript::AMapScript()
{

	PrimaryActorTick.bCanEverTick = false;

}

// Game starts
void AMapScript::BeginPlay()
{
	Super::BeginPlay();

	// Database
	TArray<AActor*>temp = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADatabaseVariablesScript::StaticClass(), temp);
	
	if (temp.IsValidIndex(0))
		DatabaseRef = Cast<ADatabaseVariablesScript>(temp[0]);
	
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Something just went wrong, reload please!"));
		return;
	}

}

void AMapScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<AActor*> AMapScript::Generate(TArray<FString> map, FVector2D size) {

	Map = TArray<FString>(map);
	MapSize = FVector2D(size);

							// 0:Pacman, 1:Blinky, 2:Inky,	3:Pinky,	4:Clyde
	TArray<AActor*> Characters = { nullptr, nullptr, nullptr, nullptr, nullptr };
	TArray<FVector2D> DefaultTargets = { FVector2D(0, 0), FVector2D(0, 0), FVector2D(0, 0), FVector2D(0, 0) };
	TArray<FVector2D> HomeTargets = { FVector2D(0, 0), FVector2D(0, 0) };

	FoodArray = TArray<FFoodPointer>();

	// Camera Position
	TArray<AActor*> camera;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CameraClass, camera);
	FVector cameraLocation = 100 * FVector(MapSize.X / 2, MapSize.Y * 0.47, MapSize.Y / 0.89);
	
	if (camera.IsValidIndex(0))
		Cast<AMyCamera>(camera[0])->setPositionAndRotation(cameraLocation, FVector(0, -90, -90));
	
	else
		Cast<AMyCamera>(GetWorld()->SpawnActor(CameraClass, &cameraLocation))->setRotation(FVector(0, -90, -90));

	for (int32 y = 0; y < MapSize.Y; y++)
		for (int32 x = 0; x < MapSize.X; x++) {

			FVector Location = 100 * FVector(x, y, 0);
			
			switch (Map[y][x]) {
			
			case 'o':
			case '@':
			{

				AFoodScript* food = Cast<AFoodScript>(GetWorld()->SpawnActor(FoodClass, &Location));
				food->setPU(Map[y][x] == '@');

				FoodArray.Add(FFoodPointer(food, FVector2D(x, y)));

				break;

			}
			
			// Wall
			case '-':
			case '=':
			{

				bool singleW = Map[y][x] == '-';
				TArray<bool> side = { y + 1 < MapSize.Y && (Map[y + 1][x] == '-' || Map[y + 1][x] == '='),
									  y > 0				&& (Map[y - 1][x] == '-' || Map[y - 1][x] == '='),
									  x + 1 < MapSize.X && (Map[y][x + 1] == '-' || Map[y][x + 1] == '='),
									  x > 0				&& (Map[y][x - 1] == '-' || Map[y][x - 1] == '=') };

				int32 Counter = 0;
				for (int32 k = 0; k < 4; k++)
					Counter += side[k] ? 1 : 0;
				
				switch (Counter) {

				case 1:
				{

					FVector Rotation;
					bool isDoor = false;

					if (side[0]) {
					
						if (y > 0 && Map[y - 1][x] == '+')
							isDoor = true;
						Rotation = FVector((!callWalkable(x - 1, y, 0) || callGhostHouse(x - 1, y) ? 0 : 180), 180, 90);
					}
					
					else if (side[1]) {
						
						if (y + 1 < MapSize.Y && Map[y + 1][x] == '+')
							isDoor = true;
						Rotation = FVector((!callWalkable(x + 1, y, 0) || callGhostHouse(x + 1, y) ? 0 : 180), 180, -90);
					}
					
					else if (side[2]) {
					
						if (x > 0 && Map[y][x - 1] == '+')
							isDoor = true;
						Rotation = FVector((!callWalkable(x, y - 1, 0) || callGhostHouse(x, y - 1) ? 0 : 180), 0, 180);
					}
					
					else {
					
						if (x + 1 < MapSize.X && Map[y][x + 1] == '+')
							isDoor = true;
						Rotation = FVector((!callWalkable(x, y + 1, 0) || callGhostHouse(x, y + 1) ? 0 : 180), 0, 0);
					}

					AWallScript* wall = Cast<AWallScript>(GetWorld()->SpawnActor(isDoor ? DoubleWallClass[5] : DoubleWallClass[0], &Location));
					wall->ChangeRotation(Rotation);

					break;
				}

				case 2:
				case 3:

					if ((side[0] && side[1]) ^ (side[2] && side[3])) {

						AWallScript* wall;
						int32 Index;

						if (callWalkable(x + 1, y, 0) || callWalkable(x - 1, y, 0) || callWalkable(x, y - 1, 0) || callWalkable(x, y + 1, 0))
							Index = 0;
						
						else
							Index = 3;

						wall = Cast<AWallScript>(GetWorld()->SpawnActor(singleW && Index == 0 ? WallClass[0] : DoubleWallClass[Index], &Location));

						if (Index == 0)
						
							if (side[0] && side[1])
								wall->ChangeRotation(FVector((side[3] || callWalkable(x + 1, y, 0) ? 0 : 180), 0, 90));
							else
								wall->ChangeRotation(FVector((side[0] || callWalkable(x, y - 1, 0) ? 0 : 180), 0, 0));
						else {
						
							if (y + 1 < MapSize.Y && Map[y + 1][x] == '-')
								wall->ChangeRotation(FVector(callWalkable(x + 1, y + 1, 0) ? 180 : 0, 0, 90));
							
							else if (y > 0 && Map[y - 1][x] == '-')
								wall->ChangeRotation(FVector(callWalkable(x - 1, y - 1, 0) ? 180 : 0, 0, -90));
							
							else if (x > 0 && Map[y][x - 1] == '-')
								wall->ChangeRotation(FVector(callWalkable(x - 1, y - 1, 0) ? 180 : 0, 180, 0));
							
							else
								wall->ChangeRotation(FVector(callWalkable(x + 1, y - 1, 0) ? 180 : 0, 0, 0));
						}

					}
					
					else {

						int32 Index;
						FVector Rotation;

						if (side[1])
							
							if (side[3]) {
								Index = callWalkable(x + 1, y, 0) || callWalkable(x, y - 1, 0) ? 1 : 2;
								Rotation = FVector(0, 0, 180);
							}
						
							else {
								Index = callWalkable(x - 1, y, 0) || callWalkable(x, y - 1, 0) ? 1 : 2;
								Rotation = FVector(0, 0, -90);
							}
						
						else if (side[3]) {
							Index = callWalkable(x + 1, y, 0) || callWalkable(x, y + 1, 0) ? 1 : 2;
							Rotation = FVector(0, 0, 90);
						}
						
						else {
							Index = callWalkable(x - 1, y, 0) || callWalkable(x, y + 1, 0) ? 1 : 2;
							Rotation = FVector(0, 0, 0);
						}

						if (callGhostHouse(x + 1, y + 1) || callGhostHouse(x - 1, y + 1) || callGhostHouse(x + 1, y - 1) || callGhostHouse(x - 1, y - 1))
							Index = 4;

						AWallScript* wall = Cast<AWallScript>(GetWorld()->SpawnActor(singleW ? WallClass[1] : DoubleWallClass[Index], &Location));
						wall->ChangeRotation(Rotation);

					}

					break;

				case 4:

					AWallScript * wall = Cast<AWallScript>(GetWorld()->SpawnActor(singleW ? WallClass[1] : DoubleWallClass[1], &Location));

					if (callWalkable(x - 1, y + 1, 0))
						wall->ChangeRotation(FVector(0, 0, 90));
					else if (callWalkable(x - 1, y - 1, 0))
						wall->ChangeRotation(FVector(0, 0, 180));
					else if (callWalkable(x + 1, y - 1, 0))
						wall->ChangeRotation(FVector(0, 0, -90));

					break;

				}

				break;
			}
			
			// Teleport
			case 'T':

				if (!isValidPosition(x + 1, y, MapSize)) {
					FVector tempLocation = Location + FVector(100, 0, 1);
					GetWorld()->SpawnActor(Empty_TileClass, &tempLocation)->SetActorRelativeScale3D(FVector(1, 1.5, 1));
				}
				else if (!isValidPosition(x - 1, y, MapSize)) {
					FVector tempLocation = Location + FVector(-100, 0, 1);
					GetWorld()->SpawnActor(Empty_TileClass, &tempLocation)->SetActorRelativeScale3D(FVector(1, 1.5, 1));
				}
				else if (!isValidPosition(x, y + 1, MapSize)) {
					FVector tempLocation = Location + FVector(0, 100, 1);
					GetWorld()->SpawnActor(Empty_TileClass, &tempLocation)->SetActorRelativeScale3D(FVector(1.5, 1, 1));
				}
				else if (!isValidPosition(x, y - 1, MapSize)) {
					FVector tempLocation = Location + FVector(0, -100, 1);
					GetWorld()->SpawnActor(Empty_TileClass, &tempLocation)->SetActorRelativeScale3D(FVector(1.5, 1, 1));
				}

				break;
			
			// Door
			case '+':
			{
				ADoorScript* door = Cast<ADoorScript>(GetWorld()->SpawnActor(DoorClass, &Location));
				FVector Rotation = FVector(0, 0, 0);

				if (callGhostHouse(x + 1, y) || callGhostHouse(x - 1, y)) {
					Rotation.Z = 90;
				
					if (callGhostHouse(x + 1, y) && (callWalkable(x - 1, y, 0) || callGhostHouse(x - 1, y) && x > 0 && Map[y][x - 1] != 'G'))
						Rotation.X = 180;
				}
				
				else if (callGhostHouse(x, y - 1) && (callWalkable(x, y + 1, 0) || callGhostHouse(x, y + 1) && y + 1 < MapSize.Y && Map[y + 1][x] != 'G'))
					Rotation.X = 180;



				door->ChangeRotation(Rotation);

				break;
			}
			
			// PacMan
			case 'M':
			{

				FVector temp = Location;
				
				if (callWalkable(x + 1, y, 1))
					temp += FVector(50, 0, 0);
				
				else if (callWalkable(x - 1, y, 1))
					temp += FVector(-50, 0, 0);
				
				else if (callWalkable(x, y + 1, 1))
					temp += FVector(0, 50, 0);
				
				else if (callWalkable(x, y - 1, 1))
					temp += FVector(0, -50, 0);

				Characters[0] = Cast<APacManScript>(GetWorld()->SpawnActor(PacManClass, &temp));
				
				Cast<APacManScript>(Characters[0])->DatabasePtr = DatabaseRef;
				Cast<APacManScript>(Characters[0])->setLocalMap(Map, MapSize);

				break;

			}
			
			// Blinky
			case 'B':
			{

				FVector temp = Location + FVector(50, 0, 0);
				Characters[1] = Cast<ABlinky>(GetWorld()->SpawnActor(GhostClass[0], &temp));

				Cast<ABlinky>(Characters[1])->DatabasePtr = DatabaseRef;
				Cast<ABlinky>(Characters[1])->currentState = DatabaseRef->DefaultGhostState;
				Cast<ABlinky>(Characters[1])->SpawnTarget = FVector2D(x, y);
				Cast<ABlinky>(Characters[1])->setLocalMap(Map, MapSize);
				Cast<ABlinky>(Characters[1])->DefinePosAround(1);	

				break;

			}
			
			// Pinky
			case 'P':
			{

				FVector temp = Location + FVector(50, 0, 0);
				Characters[2] = Cast<APinky>(GetWorld()->SpawnActor(GhostClass[1], &temp));

				Cast<APinky>(Characters[2])->DatabasePtr = DatabaseRef;
				Cast<APinky>(Characters[2])->currentState = 3;
				Cast<APinky>(Characters[2])->SpawnTarget = FVector2D(x, y);
				Cast<APinky>(Characters[2])->setLocalMap(Map, MapSize);
				Cast<APinky>(Characters[2])->DefinePosAround(2);

				break;

			}

			// Inky
			case 'I':
			{

				FVector temp = Location + FVector(50, 0, 0);
				Characters[3] = Cast<AInky>(GetWorld()->SpawnActor(GhostClass[2], &temp));

				Cast<AInky>(Characters[3])->DatabasePtr = DatabaseRef;
				Cast<AInky>(Characters[3])->currentState = 3;
				Cast<AInky>(Characters[3])->SpawnTarget = FVector2D(x, y);
				Cast<AInky>(Characters[3])->setLocalMap(Map, MapSize);
				Cast<AInky>(Characters[3])->DefinePosAround(2);

				break;

			}

			// Clyde
			case 'C':
			{

				FVector temp = Location + FVector(50, 0, 0);
				Characters[4] = Cast<AClyde>(GetWorld()->SpawnActor(GhostClass[3], &temp));

				Cast<AClyde>(Characters[4])->DatabasePtr = DatabaseRef;
				Cast<AClyde>(Characters[4])->currentState = 3;
				Cast<AClyde>(Characters[4])->SpawnTarget = FVector2D(x, y);
				Cast<AClyde>(Characters[4])->setLocalMap(Map, MapSize);
				Cast<AClyde>(Characters[4])->DefinePosAround(2);

				break;

			}
			
			// Ghost default target
			case '1': case '2': case '3': case '4':

				DefaultTargets[Map[y][x] - '1'] = FVector2D(x, y);

				break;
			
			// Home
			case 'H':

				HomeTargets[0] = FVector2D(x, y);

				break;
			
			// Homeless target 
			case 'N':

				HomeTargets[1] = FVector2D(x, y);

				break;


			default:
				
				break;

			};

		}

	if (Characters[0] != nullptr)
		Cast<APacManScript>(Characters[0])->setFoodArray(FoodArray);

	DatabaseRef->LeftOverFood = FoodArray.Num();

	for (int32 i = 1; i < Characters.Num(); i++) {
		Cast<IGhostInterface>(Characters[i])->DefaultTarget = DefaultTargets[i - 1];
		Cast<IGhostInterface>(Characters[i])->HomeTarget = HomeTargets[0];
		Cast<IGhostInterface>(Characters[i])->NoHomeTarget = HomeTargets[1];
	}

	return Characters;
			
}

bool AMapScript::callWalkable(int32 x, int32 y, int32 mode)
{
	return isValidPosition(x, y, MapSize) && isWalkable(Map[y][x], mode);
}

bool AMapScript::callGhostHouse(int32 x, int32 y)
{
	return isValidPosition(x, y, MapSize) && isGhostHouse(Map[y][x]);
}

void AMapScript::ReGenerate(TArray<FString> map, FVector2D size, TArray<AActor*> characters, bool restoreFood)
{

	Map = TArray<FString>(map);
	MapSize = FVector2D(size);
	FoodArray = TArray<FFoodPointer>();

	for (int32 y = 0; y < MapSize.Y; y++)
		for (int32 x = 0; x < MapSize.X; x++) {

			FVector Location = 100 * FVector(x, y, 0);
			
			switch (Map[y][x]) {
			
			// Food and PU
			case 'o':
			case '@':
			{

				if (restoreFood) {

					AFoodScript* food = Cast<AFoodScript>(GetWorld()->SpawnActor(FoodClass, &Location));
					food->setPU(Map[y][x] == '@');

					FoodArray.Add(FFoodPointer(food, FVector2D(x, y)));

				}

				break;

			}
			
			// Pacman spawn
			case 'M':
			{

				FVector temp = Location;
				if (callWalkable(x + 1, y, 1))
					temp += FVector(50, 0, 0);
				else if (callWalkable(x - 1, y, 1))
					temp += FVector(-50, 0, 0);
				else if (callWalkable(x, y + 1, 1))
					temp += FVector(0, 50, 0);
				else if (callWalkable(x, y - 1, 1))
					temp += FVector(0, -50, 0);

				if (characters[0] != nullptr) {
					Cast<APacManScript>(characters[0])->SetActorLocation(temp, false, nullptr, ETeleportType::TeleportPhysics);
					Cast<APacManScript>(characters[0])->StartedGame = true;
				}

				break;

			}
			
			// Blinky spawn
			case 'B':

				if (characters[1] == nullptr)
					break;

				Cast<ABlinky>(characters[1])->SetActorLocation(Location + FVector(50, 0, 0), false, nullptr, ETeleportType::TeleportPhysics);
				Cast<ABlinky>(characters[1])->UpdateState(DatabaseRef->DefaultGhostState);
				Cast<ABlinky>(characters[1])->UpdatePosAround(1);

				break;
			
			// Pinky spawn
			case 'P':

				if (characters[2] == nullptr)
					break;

				Cast<APinky>(characters[2])->SetActorLocation(Location + FVector(50, 0, 0), false, nullptr, ETeleportType::TeleportPhysics);
				Cast<APinky>(characters[2])->UpdateState(3);
				Cast<APinky>(characters[2])->UpdatePosAround(2);

				break;
			
			// Inky spawn
			case 'I':

				if (characters[3] == nullptr)
					break;

				Cast<AInky>(characters[3])->SetActorLocation(Location + FVector(50, 0, 0), false, nullptr, ETeleportType::TeleportPhysics);
				Cast<AInky>(characters[3])->UpdateState(3);
				Cast<AInky>(characters[3])->UpdatePosAround(2);

				break;
			
			// Clyde spawn
			case 'C':

				if (characters[4] == nullptr)
					break;

				Cast<AClyde>(characters[4])->SetActorLocation(Location + FVector(50, 0, 0), false, nullptr, ETeleportType::TeleportPhysics);
				Cast<AClyde>(characters[4])->UpdateState(3);
				Cast<AClyde>(characters[4])->UpdatePosAround(2);

				break;


			default:
				
				break;

			}
		
		}

	if (restoreFood) {
		if (characters[0] != nullptr)
			Cast<APacManScript>(characters[0])->setFoodArray(FoodArray);
		DatabaseRef->LeftOverFood = FoodArray.Num();
	}

}