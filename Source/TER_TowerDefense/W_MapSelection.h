// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelType.h"
#include "UA_Game.h"
#include "W_MapNode.h"
#include "W_MapSelection.generated.h"

/**
 * 
 */
class UButton;
class UImage;
class UCanvasPanel;

UCLASS()
class TER_TOWERDEFENSE_API UW_MapSelection : public UUserWidget
{
	GENERATED_BODY()
protected:	
	
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSubclassOf<UUserWidget> NodeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	UTexture2D* ArrowTexture;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MapCanvas;

	UPROPERTY(EditDefaultsOnly)
	FVector2D NodeSize = FVector2D(130.f, 130.f);
	
	void BuildMap();

	UFUNCTION()
	void HandleNodeSelected(FLevelTypeData NodeData);
	
private:
	UPROPERTY()
	UUA_Game* GameInstance;
	
	void SpawnNode(FLevelTypeData NodeData, FVector2D Position);
	void SpawnArrow(FVector2D From, FVector2D To);
	void OpenLevel(FLevelTypeData NodeData);
};