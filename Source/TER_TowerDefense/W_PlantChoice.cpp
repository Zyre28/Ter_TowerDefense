// Fill out your copyright notice in the Description page of Project Settings.


#include "W_PlantChoice.h"

#include "UA_Game.h"
#include "MC.h"
#include "LevelManager.h"
#include "W_PlantCard.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UW_PlantChoice::NativeConstruct()
{
    Super::NativeConstruct();

    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UW_PlantChoice::OnConfirmClicked);
    }
    
    UUA_Game* GI = GetUA_Game();
    if (!GI)
    {
        return;
    }

    SelectedPlants.SetNum(GI->MaxSlot);
    for (FString& SelectedPlant : SelectedPlants)
    {
        SelectedPlant = TEXT("");
        //Maybe special *Empty Card* instead ? 
        //Maybe store chosen plant from last round ? 
    }

    RefreshSlots();
    RefreshAvailablePlants();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetInputMode(FInputModeUIOnly());
        PC->SetShowMouseCursor(true);
    }
}

void UW_PlantChoice::OnPlantCardClicked(FString PlantKey)
{
    int32 ExistingIndex = SelectedPlants.IndexOfByKey(PlantKey);
    if (ExistingIndex != INDEX_NONE)
    {
        SelectedPlants[ExistingIndex] = TEXT("");
        RefreshSlots();
        RefreshAvailablePlants();
        return;
    }

    int32 FreeSlot = INDEX_NONE;
    for (int32 i = 0; i < SelectedPlants.Num(); i++)
    {
        if (SelectedPlants[i].IsEmpty())
        {
            FreeSlot = i;
            break;
        }
    }

    if (FreeSlot == INDEX_NONE)
    {
        return;
    }

    SelectedPlants[FreeSlot] = PlantKey;
    RefreshSlots();
    RefreshAvailablePlants();
}

void UW_PlantChoice::OnSlotClicked(int32 SlotIndex)
{
    if (!SelectedPlants.IsValidIndex(SlotIndex))
    {
        return;
    }
    SelectedPlants[SlotIndex] = TEXT(""); //Maybe special *Empty Card* instead ?
    RefreshSlots();
    RefreshAvailablePlants();
}

void UW_PlantChoice::OnConfirmClicked()
{
    UUA_Game* GI = GetUA_Game();
    if (!GI)
    {
        return;
    }

    for (auto& Pair : GI->PlantsData)
    {
        // <= 0 = not takable
        if (Pair.Value.Availability <= 0)
        {
            continue;
        }

        // 2 = chosen
        // 1 = takable
        Pair.Value.Availability = SelectedPlants.Contains(Pair.Key) ? 2 : 1;
    }
    
    if (AMC* MC = GI->MC.Get())
    {
        MC->RefreshSlotsFromGameInstance();
    }

    RemoveFromParent();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->SetShowMouseCursor(false);
    }
    ALevelManager* LM = GI->GetLevelManager();
    if (LM)
    {
        LM->OnPlantSelectionConfirmed.Broadcast();
    }
}

void UW_PlantChoice::RefreshSlots()
{
    if (!SlotBox || !PlantCardClass)
    {
        return;
    }

    SlotBox->ClearChildren();
    UUA_Game* GI = GetUA_Game();
    if (!GI)
    {
        return;
    }

    for (int32 i = 0; i < SelectedPlants.Num(); i++)
    {
        UW_PlantCard* _Slot = CreateWidget<UW_PlantCard>(this, PlantCardClass);
        if (!_Slot)
        {
            continue;
        }

        FString Key = SelectedPlants[i];
        FPlantData* Data = Key.IsEmpty() ? nullptr : GI->PlantsData.Find(Key);

        _Slot->InitAsSlot(i, Data, this);
        SlotBox->AddChild(_Slot);
    }
}

void UW_PlantChoice::RefreshAvailablePlants()
{
    if (!AvailablePlantsBox || !PlantCardClass)
    {
        return;
    }

    AvailablePlantsBox->ClearChildren();
    UUA_Game* GI = GetUA_Game();
    if (!GI) 
    {
        return;
    }

    for (auto& Pair : GI->PlantsData)
    {
        if (Pair.Value.Availability < 1)
        {
            continue;
        }

        UW_PlantCard* Card = CreateWidget<UW_PlantCard>(this, PlantCardClass);
        if (!Card)
        {
            continue;
        }

        bool bSelected = SelectedPlants.Contains(Pair.Key);
        Card->InitAsCard(Pair.Key, Pair.Value, bSelected, this);
        AvailablePlantsBox->AddChild(Card);
    }
}

UUA_Game* UW_PlantChoice::GetUA_Game() const
{
    return Cast<UUA_Game>(GetGameInstance());
}