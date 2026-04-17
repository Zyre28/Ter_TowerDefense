#include "D:\Ter\TER_TowerDefense\Intermediate\Build\Win64\x64\TER_TowerDefenseEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"
#include "EnemyBase.h"

#include "MC.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = Mesh;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->InitSphereRadius(25.f);
	Sphere->SetCollisionProfileName(TEXT("ProjectilePreset"));
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlap);
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

void AEnemyBase::TakeDamage_Enemy(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy [%d,%d] took : %f damages"), GridRow, GridCol, DamageAmount);
	CurrentHealth = (CurrentHealth - (DamageAmount - DamageAmount * Robustness)) > 0 ? (CurrentHealth - (DamageAmount - DamageAmount * Robustness)) : 0;
	if (CurrentHealth == 0)
	{
		OnDeath();
	}
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
	UE_LOG(LogTemp, Warning, TEXT("Overlap enabled for enemy: %d"), Mesh->GetGenerateOverlapEvents());
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlap Enemy CALLED with: %s"), *OtherActor->GetName());
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Overlapped"));

	AMC* MC = Cast<AMC>(OtherActor);
	if (MC)
	{
		MC->TakeDamage_MC(Damage);
		_sleep(10);
	}
}