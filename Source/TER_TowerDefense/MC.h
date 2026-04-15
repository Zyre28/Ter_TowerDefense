// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlantBase.h"
#include "MC.generated.h"

UCLASS()
class TER_TOWERDEFENSE_API AMC : public ACharacter
{
	GENERATED_BODY()

public:
	AMC();

protected:
	virtual void BeginPlay() override;
	

	// Input 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ActionDeplacer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ActionSauter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ActionSpawnPlant_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ActionSpawnPlant_2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Plant", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlantBase> PlantBullet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Plant", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlantBase> PlantProducer;
	
	// GridManager 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid")
	TObjectPtr<class AGridManager> GridManager;
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private :
	void Move(const struct FInputActionValue& Value);
	void Jump_();
	void SpawnPlantInGrid(TSubclassOf<class APlantBase> PlantClass);
	void SpawnPlant(const struct FInputActionValue& Value);
};
