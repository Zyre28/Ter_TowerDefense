// Fill out your copyright notice in the Description page of Project Settings.

#include "W_NewPlantPopUp.h"

#include "GenerateManager.h"
#include "UA_Game.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

UW_NewPlantPopUp::UW_NewPlantPopUp(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UW_NewPlantPopUp::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Error, TEXT("NewPlantPopUp1"));

	if (UnlockButton)
	{
		UE_LOG(LogTemp, Error, TEXT("NewPlantPopUp2"));
		UnlockButton->OnPressed.AddDynamic(this, &UW_NewPlantPopUp::OnUnlockClicked);
	}
	
	if (PlantNameText)
	{
		PlantNameText->SetText(FText::FromString(PlantName));
	}
	
	if (PlantDescriptionText)
	{
		PlantDescriptionText->SetText(FText::FromString(PlantData->PlantDescription));
	}
}

void UW_NewPlantPopUp::InitPopUp(FString _PlantName, FPlantData& _PlantData)
{
	PlantName = _PlantName;
	PlantData = &_PlantData;
}

void UW_NewPlantPopUp::OnUnlockClicked()
{
	UUA_Game* GameInstance = Cast<UUA_Game>(GetGameInstance());
	if (GameInstance)
	{
		UnlockPlant();
		
		AGenerateManager* GenerateManager = Cast<AGenerateManager>(
			UGameplayStatics::GetActorOfClass(GetWorld(), AGenerateManager::StaticClass()));
		if (GenerateManager)
		{
			GenerateManager->EndOfLevel();
		}
		
		RemoveFromParent();
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->SetShowMouseCursor(false);
		}
	}
}

void UW_NewPlantPopUp::UnlockPlant()
{
	PlantData->Availability = 1;
}
