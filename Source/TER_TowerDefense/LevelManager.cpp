// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "LevelManager.h"

#include "EnemyBasic.h"
#include "GridManager.h"
#include "MC.h"
#include "UA_Game.h"
#include "LevelType.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/UserWidget.h"


// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UUA_Game>(GetGameInstance());
	if (GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Manager Tried"));
		GameInstance->SetLevelManager(this);
	}
	
	OnPlantSelectionConfirmed.AddDynamic(
		this,
		&ALevelManager::StartLevel
	);
	ShowPlantSelectionWidget();
}

void ALevelManager::StartLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Level STARTED after confirmation"));

	FLevelTypeData LevelType = {ELevelType::Invasion, 1};
	SelectionLevel(LevelType);
}

void ALevelManager::SetMC(AMC* _MC)
{
	if (_MC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMC Setted"));
		this->MC = _MC;
	}
}

void ALevelManager::SetActualLevel(int32 _ActualLevel)
{
	this->ActualLevel = _ActualLevel;
}

int32 ALevelManager::GetActualLevel()
{
	return ActualLevel;
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALevelManager destruction"));
		GameInstance->UnSetLevelManager();
	}
}

void ALevelManager::SelectionLevel(FLevelTypeData LevelType)
{
	switch (LevelType.LevelType)
	{
		case ELevelType::Rampart : MakeLevel1(); break;
		case ELevelType::Invasion : MakeLevel1(); break;
		case ELevelType::Campfire: MakeLevel1(); break;
		case ELevelType::MutationChamber: MakeLevel1(); break;
		case ELevelType::BloodyChamber: MakeLevel1(); break;
		case ELevelType::Refuge : MakeLevel1(); break;
		default: break;
	}
}

TArray<FEnemyTypeData> ALevelManager::BuildEnemyPool(int32 CurrentWave)
{
	int32 Threshold = FMath::Max(GameInstance->GetActualDifficulty(), ActualLevel) + FMath::FloorToInt(1 + CurrentWave * 0.5f);
	
	// WIP 
    // On filtre les eligibles (rank <= threshold)
    TArray<FEnemyTypeData> Eligible;
    for (const FEnemyTypeData& Data : AllEnemyTypes)
    {
        if (Data.Rank <= Threshold)
            Eligible.Add(Data);
    }

    // On trie par rank décroissant
    Eligible.Sort([](const FEnemyTypeData& A, const FEnemyTypeData& B) {
        return A.Rank > B.Rank;
    });

    // Puis on met dans le pool uniquement MaxPoolSize Ennemie (les plus forts)
    TArray<FEnemyTypeData> Pool;
    for (int32 i = 0; i < FMath::Min(Eligible.Num(), MaxPoolSize); i++)
    {
        Pool.Add(Eligible[i]);
    }

    // Si plusieurs types ont le même rank minimal, random parmi eux
    if (Pool.Num() == MaxPoolSize && Eligible.Num() > MaxPoolSize)
    {
        int32 MinRankInPool = Pool.Last().Rank;

        // Retirer tous les types avec ce rank du pool
        Pool.RemoveAll([MinRankInPool](const FEnemyTypeData& D) {
            return D.Rank == MinRankInPool;
        });

        // Récupérer tous les candidats avec ce rank
        TArray<FEnemyTypeData> Candidates;
        for (const FEnemyTypeData& D : Eligible)
        {
            if (D.Rank == MinRankInPool)
                Candidates.Add(D);
        }

        // Piocher aléatoirement parmi eux pour compléter jusqu'à MaxPoolSize
        while (Pool.Num() < MaxPoolSize && Candidates.Num() > 0)
        {
            int32 PickedIndex = FMath::RandRange(0, Candidates.Num() - 1);
            Pool.Add(Candidates[PickedIndex]);
            Candidates.RemoveAt(PickedIndex);
        }
    }
    return Pool;
}

EEnemyType ALevelManager::PickEnemyFromPool(const TArray<FEnemyTypeData>& Pool, bool bIsEndOfWave)
{
    // Calcul du rank max dans le pool
    int32 RankMax = 0;
    for (const FEnemyTypeData& D : Pool)
        RankMax = FMath::Max(RankMax, D.Rank);

	if (!bIsEndOfWave)
	{
		RankMax+= 3;
	}
    // Poids inversé : poids = RankMax - rank + 1
    TArray<int32> Weights;
    int32 TotalWeight = 0;
    for (const FEnemyTypeData& D : Pool)
    {
        int32 W = RankMax - D.Rank + 1;
        Weights.Add(W);
        TotalWeight += W;
    }

    // Tirage aléatoire pondéré
    int32 Roll = FMath::RandRange(0, TotalWeight - 1);
    int32 Cumulative = 0;
    for (int32 i = 0; i < Pool.Num(); i++)
    {
        Cumulative += Weights[i];
        if (Roll < Cumulative)
            return Pool[i].Type;
    }
    return Pool.Last().Type; // fallback
}

void ALevelManager::MakeLevel1()
{
	int32 BaseEnemies = FMath::RandRange(20, 30);
	
	float DifficultyBonus = GameInstance->GetActualDifficulty() * 0.1f;
	float LevelMultiplier = ActualLevel * 0.1f;

	MaxEnemies = FMath::RoundToInt(BaseEnemies * (1.0f + LevelMultiplier + DifficultyBonus));
	StillEnemies = MaxEnemies;
	Waves.SetNum(NbWaves);
	
	for (int32 w = 0; w < NbWaves; w++)
	{
		TArray<FEnemyTypeData> Pool = BuildEnemyPool(w);
		FEnemyWave& Wave = Waves[w];
		int32 WaveSize = MaxEnemies * (1.0f + NbWaves*0.5);
		Wave.EnemyList.SetNum(WaveSize);
		Wave.EndOfWaves.SetNum(WaveSize);
		Wave.MaxEnemiesWave = WaveSize;
		Wave.StillEnemiesWave = WaveSize;
		Wave.StillEndOfWaves = WaveSize;


		int32 StartIndex = 0;

		// Wave 1 : 4 Basic fixes en premier
		if (w == 0)
		{
			for (int32 i = 0; i < 4 && i < WaveSize; i++)
				Wave.EnemyList[i] = EEnemyType::Basic;
			StartIndex = 4;
		}

		// Reste : tirage pondéré depuis le pool
		for (int32 i = StartIndex; i < WaveSize; i++)
			Wave.EnemyList[i] = PickEnemyFromPool(Pool, false);
		
		//EndOfWave
		for (int32 i = 0; i < 4 && i < WaveSize; i++)
		{
			Wave.EndOfWaves[i] = EEnemyType::Flag;
		}
		StartIndex = 4;
		for (int32 i = StartIndex; i < WaveSize; i++)
		{
			Wave.EndOfWaves[i] = PickEnemyFromPool(Pool, true);
		}
	}
	
	CurrentWaveIndex = 0;
	CurrentEnemyIndex = 0;
	CurrentOnScreen = 0.0f;
	bEndOfWave = false;
	TrySpawnNext();

}

TSubclassOf<AEnemyBase> ALevelManager::GetEnemyClassFromType(EEnemyType Type)
{
	TSubclassOf<AEnemyBase>* Found = EnemyClassMap.Find(Type);
	if (!Found)
	{
		UE_LOG(LogTemp, Warning, TEXT("Class notFound"));
		return nullptr;
	}
	return *Found;
}

void ALevelManager::ExecuteLevel()
{
	MakeLevel1();
}

void ALevelManager::TrySpawnNext()
{
	if (!MC || !MC->GetGenerateManager()) return;
	AGridManager* GridManager = Cast<AGridManager>(MC->GetGenerateManager());
	if (!GridManager) return;
	if (GridManager->Cells.Num() == 0) return;
	
	if (CurrentWaveIndex >= Waves.Num()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Waves ended"));
		return;
	}
	
	FEnemyWave& Wave = Waves[CurrentWaveIndex];
	TArray<EEnemyType>* List;
	int32* Still;

	if (bEndOfWave)
	{
		List = &Wave.EndOfWaves;
		Still = &Wave.StillEndOfWaves;
		UE_LOG(LogTemp, Warning, TEXT("EndOf Waves"));
	}
	else
	{
		List = &Wave.EnemyList;
		Still = &Wave.StillEnemiesWave;
		UE_LOG(LogTemp, Warning, TEXT("Normal Waves"));
	}
	
	if (*Still <= 0 || CurrentEnemyIndex >= List->Num())
	{
		return;
	}
	if (!bEndOfWave && CurrentOnScreen >= FMath::FloorToInt(MaxOnScreen))
	{
		return;
	}

	TSubclassOf<AEnemyBase> EnemyClass = GetEnemyClassFromType((*List)[CurrentEnemyIndex]);
	AEnemyBase* Enemy = GridManager->SpawnEnemy(EnemyClass);

	if (Enemy)
	{
		Enemy->OnDeathDelegate.BindUObject(this, &ALevelManager::OnEnemyDied);
		CurrentOnScreen++;
		CurrentEnemyIndex++;
	}

	if (*Still > 0 && CurrentEnemyIndex < List->Num())
	{
		float Delay = bEndOfWave ? FMath::FRandRange(0.0f, 0.2f) : FMath::FRandRange(0.0f, MaxSpawnDelay);
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ALevelManager::TrySpawnNext, Delay, false);
	}
}

void ALevelManager::OnEnemyDied()
{
	CurrentOnScreen--;
    
	// MaxOnScreen avec plafond WIP
	MaxOnScreen = FMath::Min(MaxOnScreen + 0.5f, 20.0f);

	FEnemyWave& Wave = Waves[CurrentWaveIndex];
	int32& Still = bEndOfWave ? Wave.StillEndOfWaves : Wave.StillEnemiesWave;
	Still--;

	if (Still <= 0)
	{
		if (!bEndOfWave && Wave.EndOfWaves.Num() > 0)
		{
			// Passer au EndOfWave
			bEndOfWave = true;
			CurrentEnemyIndex = 0;
		}
		else
		{
			// Passer à la vague suivante
			bEndOfWave = false;
			CurrentEnemyIndex = 0;
			CurrentWaveIndex++;
			if (CurrentWaveIndex >= Waves.Num() && CurrentOnScreen == 0)
			{
				OnLevelComplete();
				return;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Waves ended"));
	}

	TrySpawnNext();
}

void ALevelManager::OnLevelComplete()
{
	MC->GetGenerateManager()->EndOfLevel();
}

void ALevelManager::ShowPlantSelectionWidget()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !PlantSelectionWidgetClass) return;

	PlantSelectionWidget = CreateWidget<UUserWidget>(PC, PlantSelectionWidgetClass);
	if (!PlantSelectionWidget) return;

	PlantSelectionWidget->AddToViewport();

	PC->SetInputMode(FInputModeUIOnly());
	PC->SetShowMouseCursor(true);

	UE_LOG(LogTemp, Warning, TEXT("Plant Selection Widget shown"));
}