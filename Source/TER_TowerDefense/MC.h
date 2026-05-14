// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UA_Game.h"
#include "MC.generated.h"

class AGenerateManager;
class AResourceManager;
class APlantBase;

UCLASS()
class TER_TOWERDEFENSE_API AMC : public ACharacter
{
	GENERATED_BODY()
	
	bool bIsDead = false;

public:
	
	AMC();
	
	virtual ~AMC() override;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ActionSpawnPlant_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ActionSpawnPlant_4;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Plant", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APlantBase>> AvailablePlants;
	
	TArray<int32> PlantSlots;
	
	void AssignPlantToSlot(int32 SlotIndex, int32 PlantIndex);
	void ClearSlot(int32 SlotIndex);
	
	TArray<int32> GetPlantSlots() const { return PlantSlots; }
	TArray<TSubclassOf<APlantBase>> GetAvailablePlants() const { return AvailablePlants; }
	
	// GridManager 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level|Grid")
	TObjectPtr<class AGenerateManager> GenerateManager;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level|Resources")
	TObjectPtr<class AResourceManager> ResourceManager;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Stats")
	float CurrentHealth = 100.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Stats")
	float Robustness = 0.f;
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player|Stats")
	int32 GetCurrentHealth();
	
	void TakeDamage_MC(float DamageAmount);
	void OnDeath();
	bool getIsDead() { return bIsDead; }
	
	void SetCurrentHealth(int32 CurrentHealth);
	
	TObjectPtr<AGenerateManager> GetGenerateManager();
	void RefreshSlotsFromGameInstance();
private :
	void Move(const struct FInputActionValue& Value);
	void Jump_();
	void SpawnPlantInGrid(TSubclassOf<class APlantBase> PlantClass);
	
	UPROPERTY()
	UUA_Game* GameInstance;
	
	void SpawnPlantAtSlot(int32 SlotIndex);
	void SpawnPlantSlot0() { SpawnPlantAtSlot(0); }
	void SpawnPlantSlot1() { SpawnPlantAtSlot(1); }
	void SpawnPlantSlot2() { SpawnPlantAtSlot(2); }
	void SpawnPlantSlot3() { SpawnPlantAtSlot(3); }
};
