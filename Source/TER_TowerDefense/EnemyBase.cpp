#include "D:\Ter\TER_TowerDefense\Intermediate\Build\Win64\x64\TER_TowerDefenseEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"
#include "EnemyBase.h"
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("EnemyPreset"));
	RootComponent = MeshComponent;
}
void AEnemyBase::InitOnGrid(int32 Row, int32 Col, FVector WorldCenter)
{
	GridRow = Row;
	GridCol = Col;
	SetActorLocation(WorldCenter);

	// Debug : sphere verte au centre de la case
	DrawDebugSphere(GetWorld(), WorldCenter, 4.f, 8, FColor::Red, false, 30.f, 0, 0.5f);

	UE_LOG(LogTemp, Warning, TEXT("Enemy placed at Row=%d Col=%d Pos=%s"),
		Row, Col, *WorldCenter.ToString());
}

void AEnemyBase::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy [%d,%d] died"), GridRow, GridCol);
	Destroy();
}


void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}