// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBasic.h"

#include "Kismet/GameplayStatics.h"

AEnemyBasic::AEnemyBasic()
{
	this->MoveInterval = 2.0f;
	this->MoveAmount = 3;
	SetRank(1);
}

void AEnemyBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}