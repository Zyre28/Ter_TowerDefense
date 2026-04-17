// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceManager.generated.h"

UCLASS()
class TER_TOWERDEFENSE_API AResourceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceManager();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources")
	int32 CurrentPower = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources")
	int32 CurrentMoney = 100.f;

	UFUNCTION(BlueprintCallable, Category = "Resources")
	void AddResources(int32 Amount, int32 TypeResource);
	UFUNCTION(BlueprintCallable, Category = "Resources")
	bool SpendResources(int32 Cost, int32 TypeResource);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	int32 GetResource(int32 TypeResource);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	int32 GetCurrentPower();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	int32 GetCurrentMoney();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
