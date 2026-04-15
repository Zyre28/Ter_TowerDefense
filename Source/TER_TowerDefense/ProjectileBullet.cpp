// Fill out your copyright notice in the Description page of Project Settings.

#include "EngineUtils.h"
#include "ProjectileBullet.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBullet::AProjectileBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionProfileName(TEXT("ProjectilePreset"));
	Mesh->OnComponentHit.AddDynamic(this, &AProjectileBullet::OnHit);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Mesh;
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
	UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *ProjectileMovement->Velocity.ToString());
	StartLocation = GetActorLocation();
	if (SpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, StartLocation);
	}
	
}

void AProjectileBullet::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{	
	UE_LOG(LogTemp, Warning, TEXT("Bullet hit: %s"), *OtherActor->GetName());
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.ImpactPoint);
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
	UE_LOG(LogTemp, Warning, TEXT("Tick Bullet found (x,y) : %f"), ActualRange);

	if (ActualRange >= BaseRange)
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Bullet dead"));

	}
}

