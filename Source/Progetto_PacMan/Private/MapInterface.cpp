// Fill out your copyright notice in the Description page of Project Settings.


#include "MapInterface.h"

bool IMapInterface::isValidPosition(FVector2D position, FVector2D MaxLimit) {
	return isValidPosition(position.X, position.Y, MaxLimit);
}

bool IMapInterface::isValidPosition(int32 x, int32 y, FVector2D MaxLimit) {
	return x >= 0 && x < MaxLimit.X && y >= 0 && y < MaxLimit.Y;
}

bool IMapInterface::isWalkable(char tile, int32 mode) {

	switch (tile) {

	case ' ': 
	case 'o': 
	case '@': 
	case 'T':
	case 'M': 
	case 'N':
	
		return true;

	case 'G': 
	case '+': 
	case 'H':
	
		return mode == 2;

	default:
		
		return mode >= 1 && isGhostHouse(tile);

	}

}

bool IMapInterface::isGhostHouse(char tile) {

	switch (tile) {

	case 'B': 
	case 'I': 
	case 'P': 
	case 'C':
	case 'G': 
	case 'H':
	
		return true;

	default:
		
		return false;

	}

}