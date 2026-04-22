#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h" 
#include "PlantBase.generated.h"

class AResourceManager;

DECLARE_DELEGATE_TwoParams(FOnPlantDeath, int32, int32);

UCLASS()
class TER_TOWERDEFENSE_API APlantBase : public APawn
{
	GENERATED_BODY()

public:
	APlantBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Stats")
	float CurrentHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Stats")
	float Robustness = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Stats")
	float Damage = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Stats")
	int32 InitialCost = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Stats")
	int32 ActualCost = 50;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant|Stats")
	int32 TypeResource = 0;

	// Position on grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Grid")
	int32 GridRow = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Grid")
	int32 GridCol = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	USphereComponent* Sphere;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Plant")
	virtual void TakeDamage_Plant(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Plant")
	virtual void OnDeath();

	void InitOnGrid(int32 Row, int32 Col, FVector WorldCenter);
	
	AResourceManager* ResourceManager;
	void SetResourceManager(AResourceManager* RM) { ResourceManager = RM; }

	FOnPlantDeath OnDeathDelegate;
};