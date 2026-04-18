// Fill out your copyright notice in the Description page of Project Settings.


#include "UA_Game.h"
#include "LevelManager.h"
#include "MC.h"

void UUA_Game::SetLevelManager(ALevelManager* NewLevelManager)
{
	if (NewLevelManager)
	{
		LevelManager = NewLevelManager;
		if (MC)
		{
			LevelManager->SetMC(MC);
		}
	}
}

void UUA_Game::SetMC(AMC* NewMC)
{
	if (NewMC)
	{
		MC = NewMC;
		if (LevelManager)
		{
			LevelManager->SetMC(MC);
		}
	}
}

void UUA_Game::UnSetLevelManager()
{
	LevelManager = nullptr;
}

void UUA_Game::UnSetMC()
{
	MC = nullptr;
}