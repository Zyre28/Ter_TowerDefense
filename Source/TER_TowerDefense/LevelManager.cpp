// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#include "UA_Game.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UUA_Game>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetLevelManager(this);
	}
}

void ALevelManager::SetMC(AMC* _MC)
{
	if (_MC)
	{
		this->MC = MC;
	}
}

void ALevelManager::SetActualLevel(int32 _ActualLevel)
{
	this->ActualLevel = _ActualLevel;
}

int32 ALevelManager::GetActualLevel()
{
	return ActualLevel;
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ALevelManager::~ALevelManager() 
{
	if (GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALevelManager destruction"));
		GameInstance->UnSetLevelManager();	
	}
}