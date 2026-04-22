// Fill out your copyright notice in the Description page of Project Settings.


#include "UA_Game.h"
#include "LevelManager.h"
#include "MC.h"

void UUA_Game::SetLevelManager(ALevelManager* NewLevelManager)
{
	UE_LOG(LogTemp, Warning, TEXT("Tried to setManager1 "));
	if (NewLevelManager)
	{
		LevelManager = NewLevelManager;
		if (MC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to setMC to manager 2.1"));
			LevelManager->SetMC(MC);
		}
	}
}

void UUA_Game::SetMC(AMC* NewMC)
{
	UE_LOG(LogTemp, Warning, TEXT("Tried to setMC 1"));
	if (NewMC)
	{
		MC = NewMC;
		if (LevelManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to setMc to manager 2.2"));
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

int32 UUA_Game::GetActualDifficulty()
{
	return ActualDifficulty;
}