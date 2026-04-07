#include "D:\Ter\TER_TowerDefense\Intermediate\Build\Win64\x64\TER_TowerDefenseEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"
#include "EnemyBase.h"
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}