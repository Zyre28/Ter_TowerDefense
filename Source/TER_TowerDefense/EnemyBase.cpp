#include "D:\Ter\TER_TowerDefense\Intermediate\Build\Win64\x64\TER_TowerDefenseEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"
#include "EnemyBase.h"
#include "PlantBase.h"
#include "MC.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = Mesh;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->InitSphereRadius(70.f);
	Sphere->SetCollisionProfileName(TEXT("ProjectilePreset"));
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::EndOverlap);
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
	OnDeathDelegate.ExecuteIfBound();
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
	
	TimeSinceMove += DeltaTime;

	if (TimeSinceMove >= MoveInterval)
	{
		TimeSinceMove = 0.f;
		Move();
	}
	
	if (bIsAttackingMC || (bIsAttackingMC && bIsAttackingPlant))
	{
		TimeRemaining -= DeltaTime;
		if (TimeRemaining <= 0.f)
		{
			MC->TakeDamage_MC(Damage);
			TimeRemaining = AttackSpeed;
		}
	}
	else if (bIsAttackingPlant)
	{
		TimeRemaining -= DeltaTime;
		if (TimeRemaining <= 0.f)
		{
			Plant->TakeDamage_Plant(Damage);
			TimeRemaining = AttackSpeed;
		}
	}
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

	AMC* _MC = Cast<AMC>(OtherActor);
	
	if (_MC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy TargetMC"));
		bIsAttackingMC = true;
		TimeRemaining = 0;
		this->MC = _MC;
	}
	
	APlantBase* _Plant = Cast<APlantBase>(OtherActor);
	
	if (_Plant)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy TargetPlant"));
		bIsAttackingPlant = true;
		TimeRemaining = 0;
		this->Plant = _Plant;
	}
}

void AEnemyBase::EndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy END Overlapped"));
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;
	UE_LOG(LogTemp, Warning, TEXT("Enemy END Overlapped"));

	AMC* _MC = Cast<AMC>(OtherActor);
	
	if (_MC != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy UnTargetMC"));
		bIsAttackingMC = false;
		this->MC = nullptr;
	}
	
	APlantBase* _Plant = Cast<APlantBase>(OtherActor);
	
	if (_Plant != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy UnTargetPlant"));
		bIsAttackingPlant = false;
		this->Plant = nullptr;
	}
}

void AEnemyBase::Move()
{
	if (!(bIsAttackingMC || bIsAttackingPlant))
	{
		FVector NewLocation = GetActorLocation() + FVector(0.f, -MoveAmount, 0.f);
		SetActorLocation(NewLocation);

		UE_LOG(LogTemp, Warning, TEXT("Enemy moved to %s"), *NewLocation.ToString());
	}
}

void AEnemyBase::SetId(int32 _Id)
{
	Id = _Id;
}

int32 AEnemyBase::GetId()
{
	return Id;
}