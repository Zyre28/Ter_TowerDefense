#include "W_MapSelection.h"

#include "W_MapNode.h"
#include "LevelManager.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UW_MapSelection::NativeConstruct()
{
    Super::NativeConstruct();
    GameInstance = Cast<UUA_Game>(GetGameInstance());
    if (GameInstance)
    {
        if (GameInstance->GetMapLevels().Num() == 0)
            GameInstance->MakeMapLevels();
    
        if (!MapCanvas)
        {
            UE_LOG(LogTemp, Error, TEXT("MapCanvas null"));
            return;
        }
    
        BuildMap();
    }
}

void UW_MapSelection::BuildMap()
{
    if (!GameInstance || GameInstance->GetMapLevels().Num() == 0) return;

    MapCanvas->ClearChildren();

    TArray<FMapPath> Levels = GameInstance->GetMapLevels();

    float SizeY = 150.f;
    float SizeX = 200.f;
    float CenterY = (Levels.Num() * SizeY) * 0.5f;

    int32 MaxPathLength = 0;
    for (const FMapPath& P : Levels)
    {
        MaxPathLength = FMath::Max(MaxPathLength, P.Nodes.Num());
    }

    FVector2D RefugePos = FVector2D(0.f, CenterY);
    SpawnNode(Levels[0].Nodes[0], RefugePos);
    
    FVector2D CampfirePos = FVector2D(MaxPathLength * SizeX, CenterY);
    FVector2D FinalPos = FVector2D((MaxPathLength + 1) * SizeX, CenterY);

    for (int32 p = 0; p < Levels.Num(); p++)
    {
        float Y = p * SizeY;
        const TArray<FLevelTypeData>& Nodes = Levels[p].Nodes;

        FVector2D FirstMiddlePos = FVector2D(SizeX, Y);
        SpawnArrow(RefugePos, FirstMiddlePos);

            for (int32 n = 1; n < Nodes.Num(); n++)
        {
            FVector2D NodePos = FVector2D( n * SizeX, Y);

            SpawnNode(Nodes[n], NodePos);
            
            if (n > 1)
            {
                FVector2D PrevPos = FVector2D((n - 1) * SizeX, Y);
                SpawnArrow(PrevPos, NodePos);
            }
        }
        FVector2D LastPos = FVector2D((Nodes.Num() - 1) * SizeX, Y);
        SpawnArrow(LastPos, CampfirePos);
        
    }
    SpawnNode(GameInstance->GetSharedCampfire(), CampfirePos);
    SpawnArrow(CampfirePos, FinalPos);
    SpawnNode(GameInstance->GetSharedFinalGameplay(), FinalPos);
}

void UW_MapSelection::SpawnNode(FLevelTypeData NodeData, FVector2D Position)//WIP drag map or focus on current node
{
    if (!NodeWidgetClass)
    {
        return;
    }

    UW_MapNode* NodeWidget = CreateWidget<UW_MapNode>(this, NodeWidgetClass);
    if (!NodeWidget)
    {
        return;
    }
    
    NodeWidget->InitNode(NodeData);
    
    bool bIsCurrent = false;
    if (NodeData.BranchIndex == GameInstance->GetCurrentBranchIndex() && NodeData.NodeIndex == GameInstance->GetCurrentNodeIndex())
    {
        bIsCurrent = true;
    }
    else if (GameInstance->GetCurrentBranchIndex() == -3 && NodeData.BranchIndex >= 0 && NodeData.NodeIndex == GameInstance->GetCurrentNodeIndex())
    {
        bIsCurrent = true;
    }
    
    NodeWidget->SetIsCurrent(bIsCurrent);
    NodeWidget->OnNodeSelected.AddDynamic(this, &UW_MapSelection::HandleNodeSelected);
    
    UCanvasPanelSlot* _Slot = MapCanvas->AddChildToCanvas(NodeWidget);
    _Slot->SetPosition(Position);
    _Slot->SetSize(NodeSize);
    _Slot->SetAnchors(FAnchors(0.f));
}

void UW_MapSelection::SpawnArrow(FVector2D From, FVector2D To)
{
    if (!ArrowTexture) return;

    UImage* Arrow = NewObject<UImage>(MapCanvas); 
    if (!Arrow) return;
    
    FSlateBrush Brush;
    Brush.SetResourceObject(ArrowTexture);
    Arrow->SetBrush(Brush);

    UCanvasPanelSlot* _Slot = MapCanvas->AddChildToCanvas(Arrow);
    if (!_Slot) return;

    FVector2D NewFrom = From + FVector2D(NodeSize.X, NodeSize.Y * 0.5f);
    FVector2D NewTo = To + FVector2D(0.f, NodeSize.Y * 0.5f);
    FVector2D Mid = (NewFrom + NewTo) * 0.5f;

    float Distance = FVector2D::Distance(NewFrom, NewTo);
    FVector2D Dir = NewTo - NewFrom;
    float Angle = FMath::RadiansToDegrees(FMath::Atan2(Dir.Y, Dir.X));

    _Slot->SetPosition(Mid - FVector2D(Distance * 0.5f, 15.f));
    _Slot->SetSize(FVector2D(Distance, 30.f));
    _Slot->SetAnchors(FAnchors(0.f));

    Arrow->SetRenderTransformAngle(Angle);
}

void UW_MapSelection::HandleNodeSelected(FLevelTypeData NodeData)
{
    UE_LOG(LogTemp, Warning, TEXT("Node clicked"));
    if (!GameInstance)
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Current Branch/Node : %d/%d"), GameInstance->GetCurrentBranchIndex(), GameInstance->GetCurrentNodeIndex() );
    
    if (NodeData.BranchIndex >= 0)
    {
        if (GameInstance->CurrentBranchIndex == -3 && NodeData.NodeIndex == GameInstance->CurrentNodeIndex)
        {
            GameInstance->SetCurrentBranch(NodeData.BranchIndex);
        }
        
        if (GameInstance->CurrentBranchIndex == NodeData.BranchIndex && NodeData.NodeIndex == GameInstance->CurrentNodeIndex)
        {
            OpenLevel(NodeData);
        }
    }
    else if (NodeData.BranchIndex == -1)
    {
        if (GameInstance->CurrentBranchIndex == -1 && NodeData.NodeIndex == GameInstance->CurrentNodeIndex)
        {
            OpenLevel(NodeData);
        }
    }
    else if (NodeData.BranchIndex == -2)
    {
        if (GameInstance->CurrentBranchIndex == -2 && NodeData.NodeIndex == GameInstance->CurrentNodeIndex)
        {
            OpenLevel(NodeData);
        }
    }
}

void UW_MapSelection::OpenLevel(FLevelTypeData NodeData)
{
    ELevelType Type = NodeData.LevelType;
    switch (Type)
    {
    case ELevelType::Refuge : UGameplayStatics::OpenLevel(this, FName("Refuge")); break;
    case ELevelType::Invasion : UGameplayStatics::OpenLevel(this, FName("TestMap")); break;
    case ELevelType::Rampart : ; break;
    case ELevelType::Campfire : ; break;
    case ELevelType::BloodyChamber : ; break;
    case ELevelType::MutationChamber : ; break;
    default: ; break;
    }
}