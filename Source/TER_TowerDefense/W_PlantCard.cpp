#include "W_PlantCard.h"

#include "W_PlantChoice.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UW_PlantCard::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("PlantCard Constructed"));

	if (CardButton)
	{
		CardButton->OnClicked.AddDynamic(this, &UW_PlantCard::OnClicked);
		UE_LOG(LogTemp, Warning, TEXT("CardButton Constructed"));
	}
}

void UW_PlantCard::InitAsCard(FString Key, FPlantData& Data, bool bSelected, UW_PlantChoice* Owner)
{
	UE_LOG(LogTemp, Warning, TEXT("InitAsCard: Key=%s, PlantName=%s"), *Key, *Data.PlantName);
	UE_LOG(LogTemp, Warning, TEXT("OwnerWidget is: %s"), Owner ? TEXT("VALID") : TEXT("NULL"));
	PlantKey   = Key;
	OwnerWidget = Owner;
	bIsSlot    = false;
	SlotIndex  = -1;

	if (PlantNameText)
	{
		PlantNameText->SetText(FText::FromString(Data.PlantName));
	}

	if (PlantIconImage && Data.Icon)
	{
		PlantIconImage->SetBrushFromTexture(Data.Icon);
	}
}

void UW_PlantCard::InitAsSlot(int32 Index, FPlantData* Data, UW_PlantChoice* Owner)
{
	SlotIndex   = Index;
	OwnerWidget = Owner;
	bIsSlot     = true;
	PlantKey    = TEXT("");

	bool bFilled = Data != nullptr;
	UE_LOG(LogTemp, Warning, TEXT("InitAsSlot: Index=%d, Filled=%s"), Index, bFilled ? TEXT("true") : TEXT("false"));

	if (PlantNameText)
		PlantNameText->SetText(
			bFilled ? FText::FromString(Data->PlantName) : FText::FromString(TEXT("Vide"))
		);
	UE_LOG(LogTemp, Warning, TEXT("InitAsSlot: Index=%d, Filled=%s, PlantName=%s"), Index,
		bFilled ? TEXT("true") : TEXT("false"),
		bFilled ? *Data->PlantName : TEXT("none"));
	if (PlantIconImage)
	{
		if (bFilled && Data->Icon)
		{
			PlantIconImage->SetBrushFromTexture(Data->Icon);
		}
	}
}

void UW_PlantCard::OnClicked()
{
	if (!OwnerWidget)
	{
		return;
	}

	if (bIsSlot)
	{
		OwnerWidget->OnSlotClicked(SlotIndex);
	}
	else
	{
		OwnerWidget->OnPlantCardClicked(PlantKey);
	}
}