// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRapid.h"

AEnemyRapid::AEnemyRapid()
{
	this->MoveInterval = 1.0f;
	this->MoveAmount = 3;
	SetRank(2);
}
