// Fill out your copyright notice in the Description page of Project Settings.


#include "UA_Game.h"
#include "LevelManager.h"
#include "MC.h"
#include "Kismet/GameplayStatics.h"

void UUA_Game::Init()
{
	Super::Init();
	
	AllLevelTypes = {
		{ ELevelType::Invasion, 1 },
		{ ELevelType::Rampart, 1 },
		{ ELevelType::BloodyChamber, 2 },
		{ ELevelType::MutationChamber, 2 },
		{ ELevelType::Refuge, 2 },
		{ ELevelType::Campfire, 3 },
	};

	PlantsData.Add(TEXT("BP_PlantProducer_C"), FPlantData(TEXT("Plant Producer"),1));
	PlantsData.Add(TEXT("BP_PlantBullet_C"),   FPlantData(TEXT("Plant Bullet")));
	
	MakeMapLevels();
}

void UUA_Game::SetLevelManager(ALevelManager* NewLevelManager)
{
	UE_LOG(LogTemp, Warning, TEXT("Tried to setManager1 "));
	if (NewLevelManager)
	{
		LevelManager = NewLevelManager;
		if (MC.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to setMC to manager 2.1"));
			LevelManager->SetMC(MC.Get());
		}
		MakeMapLevels();
	}
}

void UUA_Game::SetMC(AMC* NewMC)
{
	UE_LOG(LogTemp, Warning, TEXT("Tried to setMC 1"));
	if (NewMC)
	{
		MC = NewMC;
		if (LevelManager.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to setMc to manager 2.2"));
			LevelManager->SetMC(MC.Get());
		}
		MC->SetCurrentHealth(CurrentHealth);
	}
}

void UUA_Game::UnSetLevelManager()
{
	LevelManager.Reset();
}

void UUA_Game::UnSetMC()
{
	MC.Reset();
}

int32 UUA_Game::GetActualDifficulty()
{
	return ActualDifficulty;
}

void UUA_Game::MakeMapLevels()
{
	UE_LOG(LogTemp, Warning, TEXT("MakeMapLevels"));

	MapLevels.Reset();
	
	// Gameplay types (type 1)
	TArray<FLevelTypeData> GameplayTypes;
	for (const FLevelTypeData& D : AllLevelTypes)
		if (D.Type == 1) GameplayTypes.Add(D);
	
	// Reward types (type 2)
	TArray<FLevelTypeData> RewardTypes;
	for (const FLevelTypeData& D : AllLevelTypes)
		if (D.Type == 2) RewardTypes.Add(D);

	FLevelTypeData Refuge = *AllLevelTypes.FindByPredicate([](const FLevelTypeData& D) {
		return D.LevelType == ELevelType::Refuge;
	});
	FLevelTypeData Campfire = *AllLevelTypes.FindByPredicate([](const FLevelTypeData& D) {
		return D.LevelType == ELevelType::Campfire;
	});

	for (int32 b = 0; b < NbBranches; b++)
	{
		FMapPath Path;
		int32 TotalNodes = FMath::RandRange(MinNodes, MaxNodes);

		Path.Nodes.Add(Refuge);
		Path.Nodes.Last().NodeIndex = 0;
		Path.Nodes.Last().BranchIndex = -1;
		Path.Nodes.Last().LastOfBranch = true;

		int32 MiddleNodes = TotalNodes - 3; // -3 pour Refuge + Campfire + Gameplay final
		int32 GroupNodes = MiddleNodes / 3; // on coupe les groupes incomplets
		int32 LeftNodes = MiddleNodes % 3;

		for (int32 g = 0; g < GroupNodes; g++)
		{
			for (int32 i = 0; i < 2; i++)
			{
				Path.Nodes.Add(GameplayTypes[FMath::RandRange(0, GameplayTypes.Num() - 1)]);
				Path.Nodes.Last().NodeIndex = Path.Nodes.Num() - 1;
				Path.Nodes.Last().BranchIndex = b;
			}
			Path.Nodes.Add(RewardTypes[FMath::RandRange(0, RewardTypes.Num() - 1)]);
			Path.Nodes.Last().NodeIndex = Path.Nodes.Num() - 1;
			Path.Nodes.Last().BranchIndex = b;
		}
		switch (LeftNodes)
		{
			case 1:
				Path.Nodes.Add(GameplayTypes[FMath::RandRange(0, GameplayTypes.Num() - 1)]);
				Path.Nodes.Last().NodeIndex = Path.Nodes.Num() - 1;
				Path.Nodes.Last().BranchIndex = b;;break;
			case 2:
				Path.Nodes.Add(GameplayTypes[FMath::RandRange(0, GameplayTypes.Num() - 1)]);
				Path.Nodes.Last().NodeIndex = Path.Nodes.Num() - 1;
				Path.Nodes.Last().BranchIndex = b;
				Path.Nodes.Add(RewardTypes[FMath::RandRange(0, RewardTypes.Num() - 1)]);
				Path.Nodes.Last().NodeIndex = Path.Nodes.Num() - 1;
				Path.Nodes.Last().BranchIndex = b;break;
			default:;break;
		}
		Path.Nodes.Last().LastOfBranch = true;
		MapLevels.Add(Path);	
	}
	
	SharedCampfire = Campfire;
	SharedCampfire.BranchIndex = -2;
	SharedCampfire.NodeIndex = 0;
	
	SharedFinalGameplay = GameplayTypes[FMath::RandRange(0, GameplayTypes.Num() - 1)];
	SharedFinalGameplay.BranchIndex = -2;
	SharedFinalGameplay.NodeIndex = 1;
}

void UUA_Game::OpenMapSelection()
{
	UGameplayStatics::OpenLevel(this, TEXT("MapSelectionLevel"));
}

TArray<FMapPath> UUA_Game::GetMapLevels()
{
	return MapLevels;
}

FLevelTypeData UUA_Game::GetSharedCampfire()
{
	return SharedCampfire;
}

FLevelTypeData UUA_Game::GetSharedFinalGameplay()
{
	return SharedFinalGameplay;
}

ALevelManager* UUA_Game::GetLevelManager()
{
	return LevelManager.Get();
}

void UUA_Game::SetCurrentBranch(int32 BranchIndex)
{
	CurrentBranchIndex = BranchIndex;
}

void UUA_Game::SetCurrentNode(int32 NodeIndex)
{
	CurrentNodeIndex = NodeIndex;
}

void UUA_Game::AddCurrentNode()
{
	CurrentNodeIndex++;
}

int32 UUA_Game::GetCurrentBranchIndex()
{
	return CurrentBranchIndex;
}
int32 UUA_Game::GetCurrentNodeIndex()
{
	return CurrentNodeIndex;
}

void UUA_Game::AdvanceNode()
{
	CurrentNodeIndex++;
}