// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LevelType.h"
#include "UA_Game.generated.h"

class ALevelManager;
class AMC;

USTRUCT(BlueprintType)
struct FMapPath
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FLevelTypeData> Nodes;
};

USTRUCT(BlueprintType)
struct FPlantData
{
	GENERATED_BODY()
	
	UPROPERTY()
	float MaxHealth = 100;
	
	UPROPERTY()
	float CurrentHealth = 100.f;
	
	UPROPERTY()
	float Robustness = 0.f;
	
	UPROPERTY()
	int32 Availability = 0; //TODO -1 if not obtainable 
	
	UPROPERTY()
	float ActionInterval_1 = 0.f;
	
	UPROPERTY()
	float ActionInterval_2 = 0.f;
	
	UPROPERTY()
	int32 ActionValue_1 = 0;
	
	UPROPERTY()
	int32 ActionValue_2 = 0;
	
	UPROPERTY()
	int32 Cost = 0;
	
	UPROPERTY()
	int32 TypeResource = 0;
	
	UPROPERTY()
	int32 Damage = 0;
	
	UPROPERTY()
	UTexture2D* Icon = nullptr;
	
	UPROPERTY()
	FString PlantName = "";
	
	UPROPERTY()
	FString PlantDescription = "hello";
	
	UPROPERTY()
	TArray<int32> Upgrades;
	
	FPlantData()
	{
		Upgrades.Init(-1, 8);
	}
	
	FPlantData(FString _PlantName)
	{
		PlantName = _PlantName;
		Upgrades.Init(-1, 8);
	}
	
	FPlantData(FString _PlantName, int32 _Availability)
	{
		PlantName = _PlantName;
		Availability = _Availability;
		Upgrades.Init(-1, 8);
	}
};

UCLASS()
class TER_TOWERDEFENSE_API UUA_Game : public UGameInstance
{
	GENERATED_BODY()
	
	TArray<FLevelTypeData> AllLevelTypes;
	
	TArray<FMapPath> MapLevels;
	
	FLevelTypeData SharedCampfire;
	FLevelTypeData SharedFinalGameplay;

	
public:
	virtual void Init() override;
	
	int32 NbBranches = 4;
	int32 MinNodes = 6;  // Refuge + 1 groupe(3) + Campfire + Gameplay = 7 minimum
	int32 MaxNodes = 13;
	
	float MaxHealth = 100.f;
	
	float CurrentHealth = 100.f;
	
	int32 MaxSlot = 2;
	
	int32 CurrentNodeIndex = 0;
	int32 CurrentBranchIndex = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Level")
	TWeakObjectPtr<ALevelManager> LevelManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Level")
	int32 NbLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Level")
	int32 ActualDifficulty = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|MC")
	TWeakObjectPtr<AMC> MC;
	
	UPROPERTY()
	TMap<FString, FPlantData> PlantsData;
	
	UFUNCTION(BlueprintCallable, Category="Game|Level")
	void SetLevelManager(ALevelManager* NewLevelManager);
	
	UFUNCTION(BlueprintCallable, Category="Game|MC")
	void SetMC(AMC* NewMC);
	
	UFUNCTION(BlueprintCallable, Category="Game|Level")
	void UnSetLevelManager();
	
	UFUNCTION(BlueprintCallable, Category="Game|MC")
	void UnSetMC();
	
	UFUNCTION(BlueprintCallable, Category="Map")
	void OpenMapSelection();
	
	void MakeMapLevels();
	
	void SetCurrentBranch(int32 BranchIndex);
	void SetCurrentNode(int32 BranchIndex);
	void AddCurrentNode();
	int32 GetCurrentBranchIndex();
	int32 GetCurrentNodeIndex();
	void AdvanceNode();
	
	TArray<FMapPath> GetMapLevels();
	FLevelTypeData GetSharedCampfire();
	FLevelTypeData GetSharedFinalGameplay();
	ALevelManager* GetLevelManager();
	
	int32 GetActualDifficulty();
};
