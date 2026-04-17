// Fill out your copyright notice in the Description page of Project Settings.


#include "UA_Game.h"

void UUA_Game::SetLevelManager(ALevelManager* newLevelManager)
{
	if (newLevelManager)
	{
		LevelManager = newLevelManager;
	}
}

void UUA_Game::SetMC(AMC* newMC)
{
	if (newMC)
	{
		MC = newMC;
	}
}