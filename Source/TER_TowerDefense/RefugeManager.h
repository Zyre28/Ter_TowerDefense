#pragma once
#include "GenerateManager.h"
#include "RefugeManager.generated.h"

UCLASS()
class ARefugeManager : public AGenerateManager
{
	
	GENERATED_BODY()

public:

	ARefugeManager();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generator|Refuge")
	UBoxComponent* EndOfLevelTrigger;
	
	virtual void OnEventTrigger (
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	bool bPopupOpened = false;
	
	UFUNCTION(BlueprintCallable, Category = "Generator|Refuge")
	virtual void OnEndOfLevelTrigger(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generator|Refuge")
	UStaticMeshComponent* EndOfLevelMesh;
	
	virtual void EndOfLevel() override;

protected:
	
	virtual void BeginPlay() override;
};
