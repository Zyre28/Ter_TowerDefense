// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectileBullet.generated.h"
UCLASS()
class TER_TOWERDEFENSE_API AProjectileBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 BaseRange = 4000;
	float Damage;
	FVector StartLocation;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;
	
	void InitDirection(FVector Direction);
};
