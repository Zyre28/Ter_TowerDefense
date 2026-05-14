// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_NewPlantPopUp.generated.h"

struct FPlantData;
class UImage;
class UTextBlock;
class UButton;
class UUA_Game;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class TER_TOWERDEFENSE_API UW_NewPlantPopUp : public UUserWidget
{
	GENERATED_BODY()
protected:	
	
	virtual void NativeConstruct() override;

public:
	
	UW_NewPlantPopUp(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(meta = (BindWidget))
	UImage* PlantIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* BackGroundImage;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlantNameText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlantDescriptionText;

	UPROPERTY(meta = (BindWidget))
	UButton* UnlockButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PopUp")
	UTexture2D* BackGroundTexture;

	UPROPERTY(BlueprintReadOnly, Category = "PopUp")
	FString PlantName;

	UPROPERTY(BlueprintReadOnly, Category = "PopUp")
	UTexture2D* PlantTexture;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* PopUpCanvas;
	
	FPlantData* PlantData = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void InitPopUp(FString _PlantName, FPlantData& _PlantData);
	
private:
	
	UFUNCTION()
	void OnUnlockClicked();

	UFUNCTION()
	void UnlockPlant();
};
