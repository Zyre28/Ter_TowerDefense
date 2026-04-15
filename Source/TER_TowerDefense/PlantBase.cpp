#include "D:\Ter\TER_TowerDefense\Intermediate\Build\Win64\x64\TER_TowerDefenseEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"
#include "PlantBase.h"

APlantBase::APlantBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("PlantPreset"));
	RootComponent = MeshComponent;
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
	Destroy();
}