#include "GenerateManager.h"

#include "GridManager.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "MC.h"

AGenerateManager::AGenerateManager()
{
    PrimaryActorTick.bCanEverTick = false;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    RootComponent = VisualMesh;

    EventTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EventTrigger"));
    EventTrigger->SetupAttachment(RootComponent);
    EventTrigger->SetGenerateOverlapEvents(true);
}

void AGenerateManager::BeginPlay()
{
    Super::BeginPlay();

    FVector MeshOrigin, MeshExtent;
    VisualMesh->GetLocalBounds(MeshOrigin, MeshExtent);

    UE_LOG(LogTemp, Warning, TEXT("MeshExtent: %s"), *MeshExtent.ToString());
}

void AGenerateManager::EndOfLevel()
{
    UE_LOG(LogTemp, Warning, TEXT("I'm Here"));
    UUA_Game* GameInstance = Cast<UUA_Game>(GetGameInstance());
    if (GameInstance)
    {
        if (GameInstance->GetCurrentBranchIndex() == -1)
        {
            if (GameInstance->GetMapLevels()[0].Nodes[GameInstance->GetCurrentNodeIndex()].LastOfBranch == true)//WIP
            {
                GameInstance->SetCurrentBranch(-3);
            }
            GameInstance->AddCurrentNode();
        }
        else if (GameInstance->GetCurrentBranchIndex() >= 0)
        {
            if (GameInstance->GetMapLevels()[GameInstance->GetCurrentBranchIndex()].Nodes[GameInstance->GetCurrentNodeIndex()].LastOfBranch == true)
            {
                GameInstance->SetCurrentBranch(-2);
                GameInstance->SetCurrentNode(0);
            }
            else
            {
                GameInstance->AddCurrentNode();
            }
        }
        else
        {
            GameInstance->AddCurrentNode();
        }
        UE_LOG(LogTemp, Warning, TEXT("I'm Here b/n : %d : %d"), GameInstance->GetCurrentBranchIndex(), GameInstance->GetCurrentNodeIndex());
        GameInstance->OpenMapSelection();
    }
}