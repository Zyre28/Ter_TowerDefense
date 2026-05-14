#pragma once

#include "CoreMinimal.h"
#include "EnemyType.h"
#include "LevelType.generated.h"

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	Invasion,
	Rampart,
	Campfire,
	MutationChamber,
	BloodyChamber,
	Refuge 
};

USTRUCT(BlueprintType)
struct FLevelTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ELevelType LevelType = ELevelType::Invasion;

	UPROPERTY(EditDefaultsOnly)
	int32 Type = 1;
	// 1 = Gameplay; 2 = RewardArea; 3 = SafeZone;....
	
	UPROPERTY(EditDefaultsOnly)
	int32 Layer = 1;
	// Number of possible different layer for this room WIP
	
	UPROPERTY(EditDefaultsOnly)
	int32 NodeIndex = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 BranchIndex = -1;  // -1 = Shared (Refuge, Campfire, Final)
	
	UPROPERTY(EditDefaultsOnly)
	bool LastOfBranch = false;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<EEnemyType> EnemyPool;
};