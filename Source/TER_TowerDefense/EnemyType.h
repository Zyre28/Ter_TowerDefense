#pragma once

#include "CoreMinimal.h"
#include "EnemyType.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Basic,
	Rapid,
};

USTRUCT(BlueprintType)
struct FEnemyTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EEnemyType Type = EEnemyType::Basic;

	UPROPERTY(EditDefaultsOnly)
	int32 Rank = 1;
};