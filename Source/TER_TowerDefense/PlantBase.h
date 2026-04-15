#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h" 
#include "PlantBase.generated.h"

UCLASS()
class TER_TOWERDEFENSE_API APlantBase : public APawn
{
	GENERATED_BODY()

public:
	APlantBase();

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

	UFUNCTION(BlueprintCallable, Category = "Plant")
	virtual void TakeDamage_Plant(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Plant")
	virtual void OnDeath();

	void InitOnGrid(int32 Row, int32 Col, FVector WorldCenter);
};