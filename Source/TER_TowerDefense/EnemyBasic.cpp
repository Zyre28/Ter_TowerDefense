// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBasic.h"

#include "Kismet/GameplayStatics.h"

void AEnemyBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceMove += DeltaTime;

	if (TimeSinceMove >= MoveInterval)
	{
		TimeSinceMove = 0.f;
		Move();
	}
}

void AEnemyBasic::Move()
{
	if (!(bIsAttackingMC || bIsAttackingPlant))
	{
		FVector NewLocation = GetActorLocation() + FVector(0.f, -MoveAmount, 0.f);
		SetActorLocation(NewLocation);

		UE_LOG(LogTemp, Warning, TEXT("Enemy moved to %s"), *NewLocation.ToString());
	}
}