// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRapid.h"

AEnemyRapid::AEnemyRapid()
{
	this->MaxHealth = 50.f;
	this->MoveInterval /= 2;
	this->MoveAmount *= 2;
}
