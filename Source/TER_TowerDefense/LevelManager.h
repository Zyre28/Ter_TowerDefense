// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UA_Game.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

UCLASS()
class TER_TOWERDEFENSE_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
	TObjectPtr<class AMC> MC;
	
	int32 ActualLevel;
	UUA_Game* GameInstance;

public:	
	// Sets default values for this actor's properties
	ALevelManager();
	virtual ~ALevelManager() override;
	
	void SetMC(class AMC* MC);

	void SetActualLevel(int32 ActualLevel);
	int32 GetActualLevel();;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
