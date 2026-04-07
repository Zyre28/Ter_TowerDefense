#include "PlantBullet.h"

APlantBullet::APlantBullet()
{
	PrimaryActorTick.bCanEverTick = true;
	FireInterval = 5.f;
	FireAmount = 25;
}

void APlantBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastBullet += DeltaTime;
	if (TimeSinceLastBullet>= FireInterval)
	{
		TimeSinceLastBullet = 0.f;
		Fire();
	}
}

void APlantBullet::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("PlantProducer [%d,%d] produced %d resources"),
		GridRow, GridCol, FireAmount);

	// Debug : flash cyan au moment de la production
	DrawDebugSphere(GetWorld(), GetActorLocation(), 6.f, 8,
		FColor::Cyan, false, 1.f, 0, 1.f);
}