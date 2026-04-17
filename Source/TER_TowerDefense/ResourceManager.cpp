// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceManager.h"

void AResourceManager::AddResources(int32 Amount, int32 TypeResource)
{
	switch (TypeResource)
	{
		case 0: CurrentPower += Amount; break;
		case 1: CurrentMoney += Amount; break;
		default : CurrentPower -= Amount; break;
	}
	UE_LOG(LogTemp, Warning, TEXT("You have now Money : %d, Power : %d"), CurrentMoney, CurrentPower);
}

bool AResourceManager::SpendResources(int32 Cost, int32 TypeResource)
{
	switch (TypeResource)
	{
	case 0:
		{
			if (CurrentPower >= Cost)
			{
				CurrentPower -= Cost;
				return true;
			}
			break;
		}
	case 1: {
			if (CurrentMoney >= Cost)
			{
				CurrentMoney -= Cost;
				return true;
			}
			break;
		}
	default :
		{
			if (CurrentPower >= Cost)
			{
				CurrentPower -= Cost;
				return true;
			}
			break;
		}
	}
	return false;
}

int32 AResourceManager::GetResource(int32 TypeResource)
{
	switch (TypeResource)
	{
		case 0: return GetCurrentPower();
		case 1: return GetCurrentMoney();
		default : return GetCurrentPower();
	}
}

int32 AResourceManager::GetCurrentMoney()
{
	return CurrentMoney;
}

int32 AResourceManager::GetCurrentPower()
{
	return CurrentPower;
}

// Sets default values
AResourceManager::AResourceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AResourceManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResourceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

