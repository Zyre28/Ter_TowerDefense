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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Bullet")
	float FireInterval = 5.f;

	// Dmg dealt each fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Bullet")
	int32 FireAmount = 25;
	
	UPROPERTY(EditAnywhere, Category = "Plant|Bullet")
	TSubclassOf<AActor> BulletClass;
	
private:
	float TimeSinceLastBullet = 0.f;

	void Fire();
};