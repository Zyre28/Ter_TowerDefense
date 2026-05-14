// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UA_Game.h"
#include "W_PlantChoice.generated.h"

class UHorizontalBox;
class UWrapBox;
class UButton;
class UTextBlock;
class UW_PlantCard;

UCLASS()
class TER_TOWERDEFENSE_API UW_PlantChoice : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:	

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SlotBox;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* AvailablePlantsBox;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConfirmButtonText;

	UPROPERTY(EditDefaultsOnly, Category = "PlantChoice")
	TSubclassOf<UW_PlantCard> PlantCardClass;

	UPROPERTY()
	TArray<FString> SelectedPlants;

	UFUNCTION()
	void OnPlantCardClicked(FString PlantKey);

	UFUNCTION()
	void OnSlotClicked(int32 SlotIndex);

private:

	UFUNCTION()
	void OnConfirmClicked();

	void RefreshSlots();
	void RefreshAvailablePlants();

	UUA_Game* GetUA_Game() const;
};