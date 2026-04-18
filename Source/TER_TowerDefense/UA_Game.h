// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UA_Game.generated.h"

class ALevelManager;
class AMC;

UCLASS()
class TER_TOWERDEFENSE_API UUA_Game : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Level")
	ALevelManager* LevelManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Level")
	int32 NbLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Level")
	int32 ActualDifficulty;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|MC")
	AMC* MC;
	
	UFUNCTION(BlueprintCallable, Category="Game|Level")
	void SetLevelManager(ALevelManager* NewLevelManager);
	
	UFUNCTION(BlueprintCallable, Category="Game|MC")
	void SetMC(AMC* NewMC);
	
	UFUNCTION(BlueprintCallable, Category="Game|Level")
	void UnSetLevelManager();
	
	UFUNCTION(BlueprintCallable, Category="Game|MC")
	void UnSetMC();
};
