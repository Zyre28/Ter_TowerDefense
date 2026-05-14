// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UA_Game.h"
#include "GameFramework/Actor.h"
#include "EnemyType.h" 
#include "LevelManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlantSelectionConfirmed);

class AEnemyBase;
class AMC;

USTRUCT()
struct FEnemyWave
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<EEnemyType> EnemyList;
	TArray<EEnemyType> EndOfWaves;
	TArray<EEnemyType> EnemyPool;
	int32 MaxEnemiesWave;
	int32 StillEnemiesWave;
	int32 StillEndOfWaves;
};

UCLASS()
class TER_TOWERDEFENSE_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
	TObjectPtr<AMC> MC;
	
	int32 ActualLevel = 1; //WIP
	
	UPROPERTY()
	UUA_Game* GameInstance;
	
	TArray<FEnemyWave> Waves;
	
	TArray<FEnemyTypeData> AllEnemyTypes = {
		{ EEnemyType::Basic, 1 },
		{ EEnemyType::Rapid, 2 },
		{ EEnemyType::Cone, 2 },
		{ EEnemyType::Bucket, 4 },
		{ EEnemyType::Tank, 6 },
	};
	
	int32 MaxPoolSize = 4;
	float MaxOnScreen = 1.0f;
	float CurrentOnScreen = 0.0f;
	
	TSubclassOf<AEnemyBase> GetEnemyClassFromType(EEnemyType Type);
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();
	
	UFUNCTION()
	void StartLevel();
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PlantSelectionWidgetClass;

	UPROPERTY()
	UUserWidget* PlantSelectionWidget;
	
	void ShowPlantSelectionWidget();
	
	void SetMC(class AMC* MC);

	void SetActualLevel(int32 ActualLevel);
	int32 GetActualLevel();
	
	void SelectionLevel(FLevelTypeData LevelType);
	
	void MakeLevel1();
	
	int8 MaxEnemies;
	int8 StillEnemies;
	int8 NbWaves = 1;
	float CurrentWaveIndex = 0;
	float CurrentEnemyIndex = 0;
	bool bEndOfWave = false;
	
	FTimerHandle SpawnTimerHandle;
	float MaxSpawnDelay = 4.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Level|Enemies")
	TMap<EEnemyType, TSubclassOf<AEnemyBase>> EnemyClassMap;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlantSelectionConfirmed OnPlantSelectionConfirmed;
	
	TArray<FEnemyTypeData> BuildEnemyPool(int32 CurrentWave);
	EEnemyType PickEnemyFromPool(const TArray<FEnemyTypeData>& Pool, bool bEndOfWave);
	
	void ExecuteLevel();
	void TrySpawnNext();
	void OnEnemyDied();
	void OnLevelComplete();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
