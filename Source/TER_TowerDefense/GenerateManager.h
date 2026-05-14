#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GenerateManager.generated.h"

class APlantBase;
class AEnemyBase;
class AResourceManager;

UCLASS(Abstract)
class TER_TOWERDEFENSE_API AGenerateManager : public AActor
{
	GENERATED_BODY()
	
	public:
	AGenerateManager();
	
	UPROPERTY()
	UBoxComponent* EventTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generator")
	UStaticMeshComponent* VisualMesh;
	
	UPROPERTY(EditInstanceOnly, Category = "Generator")
	AResourceManager* ResourceManager;
	
	UPROPERTY(EditInstanceOnly, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;
	
	UFUNCTION()
	virtual void OnEventTrigger (
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult){};
	
	virtual void EndOfLevel();
	
protected:
	virtual void BeginPlay() override;
};