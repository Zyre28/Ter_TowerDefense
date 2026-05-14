#include "RefugeManager.h"

#include "MC.h"
#include "W_NewPlantPopUp.h"
#include "W_PlantChoice.h"
#include "Blueprint/UserWidget.h"

ARefugeManager::ARefugeManager()
{/*
    EndOfLevelTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndOfLevelTrigger"));
    EndOfLevelTrigger->SetupAttachment(RootComponent);
    EndOfLevelTrigger->SetGenerateOverlapEvents(true);*/
}


void ARefugeManager::BeginPlay()
{
    Super::BeginPlay();

    FVector MeshOrigin, MeshExtent;
    VisualMesh->GetLocalBounds(MeshOrigin, MeshExtent);
    
    FVector WorldCenter = GetActorLocation();
    EventTrigger->SetWorldLocation(WorldCenter);
    EventTrigger->SetBoxExtent(MeshExtent);
    EventTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARefugeManager::OnEventTrigger);
    EventTrigger->SetHiddenInGame(false);
    EventTrigger->ShapeColor = FColor::Red;
    
    
    /*EndOfLevelMesh->GetLocalBounds(MeshOrigin, MeshExtent);
    
    EndOfLevelTrigger->SetWorldLocation(WorldCenter);
    EndOfLevelTrigger->SetBoxExtent(MeshExtent);
    EndOfLevelTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARefugeManager::OnEndOfLevelTrigger);
    EndOfLevelTrigger->SetHiddenInGame(false);
    EndOfLevelTrigger->ShapeColor = FColor::Red;*/
}

void ARefugeManager::OnEventTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult) 
{
    
    if (bPopupOpened)
    {
        return;
    }
    
    AMC* MC = Cast<AMC>(OtherActor);
    if (!MC) return;
    
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && WidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("EventTriggered"));
        UW_NewPlantPopUp* Widget = CreateWidget<UW_NewPlantPopUp>(PC, WidgetClass);
        if (Widget)
        {
            UUA_Game* GameInstance = Cast<UUA_Game>(PC->GetGameInstance());
            if (GameInstance)
            {
                FPlantData* Data = GameInstance->PlantsData.Find(TEXT("BP_PlantBullet_C"));
                if (Data)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Data found : InitPopUp"));
                    Widget->InitPopUp("BP_PlantBullet_C", *Data);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Data is NULL"));
                }
            }
            UE_LOG(LogTemp, Warning, TEXT("Widget added to viewport"));
            if (PC)
            {
                Widget->AddToViewport();
                
                PC->SetShowMouseCursor(true);
                PC->SetInputMode(FInputModeUIOnly());
                PC->FlushPressedKeys();
                bPopupOpened = true;
            }
        }
        UW_PlantChoice* Widget2 = CreateWidget<UW_PlantChoice>(PC, WidgetClass);
        if (Widget2)
        {
            UUA_Game* GameInstance = Cast<UUA_Game>(PC->GetGameInstance());
            if (GameInstance)
            {
                UE_LOG(LogTemp, Warning, TEXT("Data found : Init"));
            }
            UE_LOG(LogTemp, Warning, TEXT("Widget2 added to viewport"));
            if (PC)
            {
                Widget2->AddToViewport();
                
                PC->SetShowMouseCursor(true);
                PC->SetInputMode(FInputModeUIOnly());
                PC->FlushPressedKeys();
                bPopupOpened = true;
            }
        }
    }
}

void ARefugeManager::EndOfLevel()
{
    Super::EndOfLevel();
}

void ARefugeManager::OnEndOfLevelTrigger(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult)
{
}