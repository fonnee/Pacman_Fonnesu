// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameNotStarted = true;

	// Map
	TArray<AActor*> temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMapScript::StaticClass(), temp);
	if (temp.IsValidIndex(0))
		Map = Cast<AMapScript>(temp[0]);
	
	else {
		UE_LOG(LogTemp, Warning, TEXT("MyGameMode: ERROR: Missing BP_Map!"));
	
		return;
	}

	// Database
	temp = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADatabaseVariablesScript::StaticClass(), temp);
	if (temp.IsValidIndex(0))
		Database = Cast<ADatabaseVariablesScript>(temp[0]);
	
	else {
		UE_LOG(LogTemp, Warning, TEXT("MyGameMode: ERROR: Missing DatabaseVariables!"));
	
		return;
	}

	// Database loading
	if (!Database->LoadAll()) {
		UE_LOG(LogTemp, Warning, TEXT("MyGameMode: ERROR: Failed to Load Infos!"));
	
		return;
	}

	Score = 0;
	HighScore = 0;
	GameTime = 0;

	// HighScore
	UseFile(true);

	
	Database->DefaultGhostState = 1;
	Database->LivesLeft = 3;

	StateCounter = 0;

	// Map
	Characters = Map->Generate(Database->Map, Database->MapSize);

	localTotalFood = Database->LeftOverFood;
	localLeftOverFood = Database->LeftOverFood;

	// Game starts
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {

		Database->canPacManMove = true;
		Database->canGhostsMove = true;

		if (Database->GhostModeTimers.IsValidIndex(0))
			GetWorldTimerManager().SetTimer(TimerHandle_GhostMode, [&]() { hasChangedMode = true; }, Database->GhostModeTimers[0], false);

		GetWorldTimerManager().SetTimer(GlobalTimerHandle, [&]() { GameTime++; }, 1.0f, true);

		GameNotStarted = false;

	}, 5.0f, false);

}

void AMyGameMode::UseFile(bool ReadMode) {

	FString file = FPaths::ProjectConfigDir();
	file.Append(TEXT("MyConfig.txt"));

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	// Check file exists
	if (FileManager.FileExists(*file))
	{

		if (ReadMode) {

			FString FileContent;

			if (FFileHelper::LoadFileToString(FileContent, *file, FFileHelper::EHashOptions::None)) {

				UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Successful Read, File Content: %s"), *FileContent);

				for (int32 i = 0; i < FileContent.Len(); i++)
					HighScore = (FileContent[i] - '0') * pow(10, FileContent.Len() - 1 - i);

			}

			else
				UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file."));

		}
		
		else {

			if (FFileHelper::SaveStringToFile(FString::FromInt(HighScore), *file)) {
				UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Successful Write."));
			}
		
			else
				UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write FString to file."));

		}
		
	}
	
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
	}

}

void AMyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameNotStarted)

		return;

	for (int32 i = 0; i < Characters.Num(); i++)
		if (!Cast<IMoveScript>(Characters[i])->DatabasePtr->IsValidLowLevel())
			Cast<IMoveScript>(Characters[i])->DatabasePtr = Database;

	
	if (!GetWorldTimerManager().IsTimerActive(GlobalTimerHandle))
		GetWorldTimerManager().SetTimer(GlobalTimerHandle, [&]() { GameTime++; }, 1.0f, true);

	// Pacman eats all food 
	if (Database->LeftOverFood <= 0) {

		
		Database->LeftOverFood = 100000;

		if (GetWorldTimerManager().IsTimerActive(TimerHandle_GhostMode))
			GetWorldTimerManager().ClearTimer(TimerHandle_GhostMode);
		if (GetWorldTimerManager().IsTimerActive(TimerHandle_GhostFrightened))
			GetWorldTimerManager().ClearTimer(TimerHandle_GhostFrightened);

		Database->PU = false;
		Database->isPacManPU = false;
		Database->GhostsEaten = 0;
		localPoint = 0;
		localGhostsEaten = Database->GhostsEaten;

		// Highscore
		UseFile(false);

		Database->canPacManMove = false;
		Database->canGhostsMove = false;

		FTimerHandle TimerHandle1, TimerHandle2, TimerHandle3;

		GetWorldTimerManager().SetTimer(TimerHandle1, [&]() {

			for (int32 i = 0; i < Characters.Num(); i++)
				Characters[i]->SetActorLocation(Characters[i]->GetActorLocation() + FVector(0, 0, -3), false, nullptr, ETeleportType::TeleportPhysics);

			}, 2.0f, false);

		// Labyrinth
		GetWorldTimerManager().SetTimer(TimerHandle2, [&]() {

			Map->ReGenerate(Database->Map, Database->MapSize, Characters, true);

			localTotalFood = Database->LeftOverFood;
			localLeftOverFood = Database->LeftOverFood;

			}, 3.0f, false);

		// Start new game
		GetWorldTimerManager().SetTimer(TimerHandle3, [&]() {

			Database->canPacManMove = true;
			Database->canGhostsMove = true;

			GameTime = 0;

			}, 5.0f, false);

	}

	if (hasChangedMode) {

		hasChangedMode = false;

		Database->DefaultGhostState = 1 - Database->DefaultGhostState;
		
		if (StateCounter < Database->GhostModeTimers.Num() - 1 && Database->GhostModeTimers[StateCounter] > 0) {

			StateCounter++;

			GetWorldTimerManager().SetTimer(TimerHandle_GhostMode, [&]() { hasChangedMode = true; }, Database->GhostModeTimers[StateCounter], false);

		}

	}

	// If Pacman was eaten
	if (Database->PacManLost) {

		if (GetWorldTimerManager().IsTimerActive(TimerHandle_GhostMode))
			GetWorldTimerManager().ClearTimer(TimerHandle_GhostMode);
		
		if (GetWorldTimerManager().IsTimerActive(TimerHandle_GhostFrightened))
			GetWorldTimerManager().ClearTimer(TimerHandle_GhostFrightened);

		// Highscore
		UseFile(false);

		Database->PacManLost = false;
		Database->canPacManMove = false;
		Database->canGhostsMove = false;

		FTimerHandle TimerHandle1, TimerHandle2, TimerHandle3;

		GetWorldTimerManager().SetTimer(TimerHandle1, [&]() {

			for (int32 i = 0; i < Characters.Num(); i++)
				Characters[i]->SetActorLocation(Characters[i]->GetActorLocation() + FVector(0, 0, -3), false, nullptr, ETeleportType::TeleportPhysics);

			}, 2.0f, false);

		if (Database->LivesLeft <= 0) {

			GetWorldTimerManager().SetTimer(TimerHandle2, [&]() {

				TArray<AActor*> temp;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCamera::StaticClass(), temp);
				if (temp.IsValidIndex(0))
					Cast<USceneComponent>(temp[0]->GetDefaultSubobjectByName(TEXT("GameOverText")))->SetVisibility(true);

				GameTime = 0;

				}, 5.0f, false);

		}
		else {

			Database->PU = false;
			Database->isPacManPU = false;
			Database->GhostsEaten = 0;
			localPoint = 0;
			localGhostsEaten = Database->GhostsEaten;

			// Labyrinth
			GetWorldTimerManager().SetTimer(TimerHandle2, [&]() {

				Database->LivesLeft--;

				Map->ReGenerate(Database->Map, Database->MapSize, Characters, false);

				}, 4.0f, false);

			// Game start
			GetWorldTimerManager().SetTimer(TimerHandle3, [&]() {

				Database->canPacManMove = true;
				Database->canGhostsMove = true;

				GameTime = 0;

				GetWorldTimerManager().SetTimer(TimerHandle_GhostMode, [&]() { hasChangedMode = true; }, Database->GhostModeTimers[StateCounter], false);

				}, 6.0f, false);
		}

	}

	// Pacman eat food
	if (localLeftOverFood > Database->LeftOverFood) {
		
		Score += 10 * (localLeftOverFood - Database->LeftOverFood);
		localPoint += localLeftOverFood - Database->LeftOverFood;
		localLeftOverFood = Database->LeftOverFood;
		
		if (Score > HighScore)
			HighScore = Score;

		GameTime = 0;

		// It was PU
		if (Database->PU) {

			Database->PU = false;
			Database->isPacManPU = true;

			if (GetWorldTimerManager().IsTimerActive(TimerHandle_GhostFrightened))
				GetWorldTimerManager().ClearTimer(TimerHandle_GhostFrightened);

			Database->GhostsEaten = 0;
			localGhostsEaten = Database->GhostsEaten;

			// Frightened state
			for (int32 i = 1; i < Characters.Num(); i++)
				if (Cast<IGhostInterface>(Characters[i])->currentState < 3)
					Cast<IGhostInterface>(Characters[i])->UpdateState(2);

			GetWorldTimerManager().SetTimer(TimerHandle_GhostFrightened, [&]() {

				Database->PU = false;
				Database->isPacManPU = false;
				Database->GhostsEaten = 0;
				localGhostsEaten = Database->GhostsEaten;

				// Reset ghosts state
				for (int32 i = 1; i < Characters.Num(); i++)
					
					if (Cast<IGhostInterface>(Characters[i])->currentState == 2)
						Cast<IGhostInterface>(Characters[i])->UpdateState(Database->DefaultGhostState);

			}, Database->Settings[14], false);

		}

	}

	// Ghost can exit to the house (Inky, Pinky and Clyde)
	for (int32 i = 2; i < Characters.Num(); i++)
		if (Cast<IGhostInterface>(Characters[i])->currentState == 3 && Cast<IGhostInterface>(Characters[i - 1])->currentState < 3 && (localPoint >= Database->GhostFree[i - 2] || GameTime >= Database->GhostFree[i + 1]))
			Cast<IGhostInterface>(Characters[i])->UpdateState(5);

	// If Pacman eat ghost
	if (localGhostsEaten < Database->GhostsEaten) {

		Score += 100 * FMath::Pow(2.0f, (float)Database->GhostsEaten);
		localGhostsEaten = Database->GhostsEaten;

		if (Score > HighScore)
			HighScore = Score;

		// Pacman eat all ghosts (-1 because in characters there is Pacman)
		if (localGhostsEaten >= Characters.Num() - 1) {

			if (GetWorldTimerManager().IsTimerActive(TimerHandle_GhostFrightened))
				GetWorldTimerManager().ClearTimer(TimerHandle_GhostFrightened);

			Database->PU = false;
			Database->isPacManPU = false;
			Database->GhostsEaten = 0;
			localGhostsEaten = Database->GhostsEaten;

		}

	}

}

int32 AMyGameMode::getScore()
{
	return Score;
}

int32 AMyGameMode::getHighScore()
{
	return HighScore;
}

int32 AMyGameMode::getPacManLives()
{
	return Database->LivesLeft;
}
