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
	
	if (UUA_Game* GameInstance = Cast<UUA_Game>(GetGameInstance()))
	{
		GameInstance->SetLevelManager(this);
	}
	
	MC = Cast<AMC>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (MC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found MC: %s"), *MC->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MC NOT FOUND"));
	}
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

