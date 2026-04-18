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
	
	AEnemyBasic();
	
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere)
	USoundBase* GettingHitSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;
};
