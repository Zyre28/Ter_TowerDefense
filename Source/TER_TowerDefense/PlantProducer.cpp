#include "PlantProducer.h"
#include "ResourceManager.h"
APlantProducer::APlantProducer()
{
	PrimaryActorTick.bCanEverTick = true;
	ProductionInterval = 5.f;
	ProductionAmount = 25;
}

void APlantProducer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastProduction += DeltaTime;
	if (TimeSinceLastProduction >= ProductionInterval)
	{
		TimeSinceLastProduction = 0.f;
		Produce();
	}
}

void APlantProducer::Produce()
{
	if (!ResourceManager)
	{
		UE_LOG(LogTemp, Error, TEXT("PlantProducer [%d,%d]: ResourceManager failed"),
			GridRow, GridCol);
		return;
	}
	ResourceManager->AddResources(ProductionAmount, ProducedResourceType);
	UE_LOG(LogTemp, Warning, TEXT("PlantProducer [%d,%d] produced %d resources"),
		GridRow, GridCol, ProductionAmount);

	// Debug : flash cyan each Production
	DrawDebugSphere(GetWorld(), GetActorLocation(), 6.f, 8,
		FColor::Cyan, false, 1.f, 0, 1.f);
}