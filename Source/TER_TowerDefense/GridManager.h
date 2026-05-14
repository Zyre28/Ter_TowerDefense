// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerateManager.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GridManager.generated.h"

class APlantBase;
class AEnemyBase;
class AResourceManager;

USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Row = 0;

	UPROPERTY()
	int32 Col = 0;
	
	UPROPERTY()
	class APlantBase* OccupyingPlant = nullptr;
	
	
	bool IsEmpty() const
	{
		return OccupyingPlant == nullptr;
	}
	
	FVector GetWorldCenter(FVector GridOrigin, float CellSizeX, float CellSizeY) const
	{
		return GridOrigin + FVector((Col + 0.5f) * CellSizeX, (Row + 0.5f) * CellSizeY, 0.f);
	}
};

UCLASS()
class TER_TOWERDEFENSE_API AGridManager : public AGenerateManager
{
	GENERATED_BODY()
	
	public:
	AGridManager();
	
	
	void InitGameplay();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 NumRows = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 NumCols = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSizeX = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSizeY = 10.f;

	UPROPERTY()
	TArray<FGridCell> Cells;

	FVector GridOrigin = FVector::ZeroVector;

	virtual void BeginPlay() override;

	FGridCell* GetCellFromWorld(FVector WorldPos);
	bool PlacePlant(class APlantBase* Plant, int32 Row, int32 Col);
	void RemovePlant(int32 Row, int32 Col);
	bool IsValidCell(int32 Row, int32 Col) const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	APlantBase* SpawnPlant(TSubclassOf<APlantBase> PlantClass, int32 Row, int32 Col);
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	AEnemyBase* SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass, int32 Row, int32 Col);
	AEnemyBase* SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass);
	
	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector SnapToGrid(FVector WorldPos);
	
	UFUNCTION(BlueprintCallable, Category="Grid")
	bool WorldToGrid(FVector WorldPos, int32& OutRow, int32& OutCol);
	
	virtual void OnEventTrigger (
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
