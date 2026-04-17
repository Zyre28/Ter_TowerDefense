// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager.h"
#include "Engine/GameInstance.h"
#include "UA_Game.generated.h"

/**
 * 
 */
UCLASS()
class TER_TOWERDEFENSE_API UUA_Game : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Level")
	ALevelManager* LevelManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|MC")
	AMC* MC;
	
	UFUNCTION(BlueprintCallable, Category="Game|Level")
	void SetLevelManager(ALevelManager* newLevelManager);
	
	UFUNCTION(BlueprintCallable, Category="Game|MC")
	void SetMC(AMC* newMC);
};
