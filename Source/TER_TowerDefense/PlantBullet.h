#pragma once
#include "PlantBase.h"
#include "PlantBullet.generated.h"

UCLASS()
class TER_TOWERDEFENSE_API APlantBullet : public APlantBase
{
	GENERATED_BODY()

public:
	APlantBullet();

	virtual void Tick(float DeltaTime) override;
	
	// Interval in secondes between each fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Producer")
	float FireInterval = 5.f;

	// Dmg dealt each fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Producer")
	int32 FireAmount = 25;
	
private:
	float TimeSinceLastBullet = 0.f;

	void Fire();
};