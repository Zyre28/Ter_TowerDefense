// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBullet.h"

#include "EnemyBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBullet::AProjectileBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(false);
	RootComponent = Mesh;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->InitSphereRadius(25.f);
	Sphere->SetCollisionProfileName(TEXT("ProjectilePreset"));
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBullet::OnOverlap);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Mesh;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Overlap enabled for Projectile: %d"), Mesh->GetGenerateOverlapEvents());
	//UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *ProjectileMovement->Velocity.ToString());
	StartLocation = GetActorLocation();
	if (SpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, StartLocation);
	}
	
}

void AProjectileBullet::OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult)
{	
	//UE_LOG(LogTemp, Warning, TEXT("OnOverlap Projectile CALLED with"));
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;
	//UE_LOG(LogTemp, Warning, TEXT("Projectile Overlapped : %s"), *OtherActor->GetName());
	
	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
	if (Enemy)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Projectile Overlapped an Enemy: %s"), *OtherActor->GetName());
		Enemy->TakeDamage_Enemy(Damage);
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		Destroy();
	}
}

void AProjectileBullet::InitDirection(FVector Direction)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
		UE_LOG(LogTemp, Warning, TEXT("InitDirection: %s"), *ProjectileMovement->Velocity.ToString());
	}
}

// Called every frame
void AProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float ActualRange = FVector::Dist(StartLocation, GetActorLocation());
	//UE_LOG(LogTemp, Warning, TEXT("Tick Bullet found (x,y) : %f"), ActualRange);

	if (ActualRange >= BaseRange)
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Bullet dead"));

	}
}

