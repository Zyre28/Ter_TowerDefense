#pragma once
#include "EnemyBase.generated.h"

UCLASS(Abstract)
class TER_TOWERDEFENSE_API AEnemyBase : public AActor
{
	GENERATED_BODY()

public:
	AEnemyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Stats")
	float Robustness = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Stats")
	float Damage = 10.f;

	// Position on grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Grid")
	int32 GridRow = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Grid")
	int32 GridCol = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant")
	UStaticMeshComponent* MeshComponent;

	virtual void BeginPlay() override;
};