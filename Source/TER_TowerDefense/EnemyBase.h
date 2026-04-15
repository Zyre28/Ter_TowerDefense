#pragma once
#include "EnemyBase.generated.h"

UCLASS(Abstract)
class TER_TOWERDEFENSE_API AEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	AEnemyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float Robustness = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float Damage = 10.f;

	// Position on grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Grid")
	int32 GridRow = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Grid")
	int32 GridCol = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UStaticMeshComponent* MeshComponent;

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaTime ) override;
	
	UFUNCTION(BlueprintCallable, Category = "Plant")
	virtual void OnDeath();

	void InitOnGrid(int32 Row, int32 Col, FVector WorldCenter);
};