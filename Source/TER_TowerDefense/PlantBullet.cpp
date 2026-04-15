#include "PlantBullet.h"

#include "ProjectileBullet.h"

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
	if (!BulletClass) return;

	FVector Origin, Extent;
	MeshComponent->GetLocalBounds(Origin, Extent);
    
	FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, Extent.Z);
	FRotator SpawnRotation = FRotationMatrix::MakeFromX(FVector::RightVector).Rotator();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AProjectileBullet* Bullet = GetWorld()->SpawnActor<AProjectileBullet>(
		BulletClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (Bullet)
	{
		Bullet->InitDirection(FVector::RightVector);
		UE_LOG(LogTemp, Warning, TEXT("Plant [%d,%d] fired a bullet of %d damages"),
			GridRow, GridCol, FireAmount);
	}
}