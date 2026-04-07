#pragma once

#include "PlantBase.h"
#include "PlantProducer.generated.h"

UCLASS()
class TER_TOWERDEFENSE_API APlantProducer : public APlantBase
{
	GENERATED_BODY()

public:
	APlantProducer();

	// Intervalle en secondes entre chaque production
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Producer")
	float ProductionInterval = 5.f;

	// Quantite produite a chaque tick de production
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Producer")
	int32 ProductionAmount = 25;

	virtual void Tick(float DeltaTime) override;

private:
	float TimeSinceLastProduction = 0.f;

	void Produce();
};