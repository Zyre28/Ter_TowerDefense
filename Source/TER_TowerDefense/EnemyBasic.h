// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyBasic.generated.h"

/**
 * 
 */
UCLASS()
class TER_TOWERDEFENSE_API AEnemyBasic : public AEnemyBase
{
	GENERATED_BODY()
	
	virtual void Tick(float DeltaTime) override;
public:
	// Interval in secondes between each move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float MoveInterval = 2.f;

	// Move lenght each move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	int32 MoveAmount = 10;
	
	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere)
	USoundBase* GettingHitSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;
	
private:
	float TimeSinceMove = 0.f;

	void Move();
};
