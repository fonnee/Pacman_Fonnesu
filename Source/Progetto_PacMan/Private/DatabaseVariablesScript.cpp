// Fill out your copyright notice in the Description page of Project Settings.


#include "DatabaseVariablesScript.h"


ADatabaseVariablesScript::ADatabaseVariablesScript()
{
 
	PrimaryActorTick.bCanEverTick = false;

}

bool ADatabaseVariablesScript::LoadAll()
{

	canPacManMove = false;
	canGhostsMove = false;
	
	PacManLost = false;
	PU = false;
	isPacManPU = false;
	GhostsEaten = 0;

	MapSize = FVector2D(28, 33);

	Map = TArray<FString>();
	Map.Add("2||||||||||||||||||||||||||1");
	Map.Add("============================");
	Map.Add("=oooooooooooo--oooooooooooo=");
	Map.Add("=o----o-----o--o-----o----o=");
	Map.Add("=@-||-o-|||-o--o-|||-o-||-@=");
	Map.Add("=o----o-----o--o-----o----o=");
	Map.Add("=oooooooooooooooooooooooooo=");
	Map.Add("=o----o--o--------o--o----o=");
	Map.Add("=o----o--o--------o--o----o=");
	Map.Add("=oooooo--oooo--oooo--oooooo=");
	Map.Add("======o----- -- -----o======");
	Map.Add("|||||=o----- -- -----o=|||||");
	Map.Add("|||||=o--    BN    --o=|||||");
	Map.Add("|||||=o-- ===++=== --o=|||||");
	Map.Add("======o-- =GGGGGG= --o======");
	Map.Add("TTTTTTo   =IGPHCG=   oTTTTTT");
	Map.Add("======o-- =GGGGGG= --o======");
	Map.Add("|||||=o-- ======== --o=|||||");
	Map.Add("|||||=o--          --o=|||||");
	Map.Add("|||||=o-- -------- --o=|||||");
	Map.Add("======o-- -------- --o======");
	Map.Add("=oooooooooooo--oooooooooooo=");
	Map.Add("=o----o-----o--o-----o----o=");
	Map.Add("=o----o-----o--o-----o----o=");
	Map.Add("=@oo--oooooooM ooooooo--oo@=");
	Map.Add("=--o--o--o--------o--o--o--=");
	Map.Add("=--o--o--o--------o--o--o--=");
	Map.Add("=oooooo--oooo--oooo--oooooo=");
	Map.Add("=o----------o--o----------o=");
	Map.Add("=o----------o--o----------o=");
	Map.Add("=oooooooooooooooooooooooooo=");
	Map.Add("============================");
	Map.Add("4||||||||||||||||||||||||||3");

	Settings = { 0, 100, 10, 80, 71, 75, 40, 20, 80, 10, 85, 90, 79, 50, 6, 5, 150 };
	GhostModeTimers = { 7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 20.0f, 5.0f, -1.0f };
	GhostFree = { 0.0f, 30.0f, 90.0f, 7.0f, 17.0f, 32.0f };

	return true;

}


void ADatabaseVariablesScript::BeginPlay()
{
	Super::BeginPlay();
	
}


void ADatabaseVariablesScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ADatabaseVariablesScript::GetIndexSprite()
{
	return Settings[0];
}