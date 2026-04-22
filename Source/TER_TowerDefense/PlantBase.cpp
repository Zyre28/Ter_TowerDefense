#include "PlantBase.h"

APlantBase::APlantBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = Mesh;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->InitSphereRadius(70.f);
	Sphere->SetCollisionProfileName(TEXT("PlantPreset"));
	Sphere->SetGenerateOverlapEvents(true);
	
	ActualCost = InitialCost;
}

void APlantBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void APlantBase::InitOnGrid(int32 Row, int32 Col, FVector WorldCenter)
{
	GridRow = Row;
	GridCol = Col;
	SetActorLocation(WorldCenter);

	// Debug : sphere verte au centre de la case
	DrawDebugSphere(GetWorld(), WorldCenter, 4.f, 8, FColor::Green, false, 30.f, 0, 0.5f);

	UE_LOG(LogTemp, Warning, TEXT("Plant placed at Row=%d Col=%d Pos=%s"),
		Row, Col, *WorldCenter.ToString());
}

void APlantBase::TakeDamage_Plant(float Amount)
{
	float Absorbed = Amount * (Robustness / 100.f);
	CurrentHealth -= (Amount - Absorbed);

	UE_LOG(LogTemp, Warning, TEXT("Plant [%d,%d] took %.1f dmg, HP=%.1f"),
		GridRow, GridCol, Amount - Absorbed, CurrentHealth);

	if (CurrentHealth <= 0.f)
		OnDeath();
}

void APlantBase::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Plant [%d,%d] died"), GridRow, GridCol);
	OnDeathDelegate.ExecuteIfBound(GridRow, GridCol);
	Destroy();
}