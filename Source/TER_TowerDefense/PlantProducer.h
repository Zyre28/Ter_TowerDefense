#pragma once

#include "PlantBase.h"
#include "PlantProducer.generated.h"

UCLASS()
class TER_TOWERDEFENSE_API APlantProducer : public APlantBase
{
	GENERATED_BODY()

public:
	APlantProducer();

	// Interval in secondes between each production
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Producer")
	float ProductionInterval = 5.f;

	// Quantity produced each production
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Producer")
	int32 ProductionAmount = 25;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Producer")
	int32 ProducedResourceType = 0;
	virtual void Tick(float DeltaTime) override;

private:
	float TimeSinceLastProduction = 0.f;
	void Produce();
};