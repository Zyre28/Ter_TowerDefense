#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UA_Game.h"
#include "W_PlantCard.generated.h"

class UImage;
class UButton;
class UTextBlock;
class UW_PlantChoice;

UCLASS()
class TER_TOWERDEFENSE_API UW_PlantCard : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY(meta = (BindWidget))
	UButton* CardButton;

	UPROPERTY(meta = (BindWidget))
	UImage* PlantIconImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlantNameText;

	// Init quand utilisé comme carte dans la zone du bas
	void InitAsCard(FString Key, FPlantData& Data, bool bSelected, UW_PlantChoice* Owner);

	// Init quand utilisé comme slot dans la zone du haut
	void InitAsSlot(int32 Index, FPlantData* Data, UW_PlantChoice* Owner);

private:

	FString PlantKey;
	int32 SlotIndex = -1;
	bool bIsSlot = false;

	UPROPERTY()
	UW_PlantChoice* OwnerWidget = nullptr;

	UFUNCTION()
	void OnClicked();
};