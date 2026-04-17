#pragma once
#include "Components/SphereComponent.h"
#include "EnemyBase.generated.h"

UCLASS(Abstract)
class TER_TOWERDEFENSE_API AEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	AEnemyBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float Robustness = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float Damage = 10.f;

	// Position on grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Grid")
	int32 GridRow = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Grid")
	int32 GridCol = 0;

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaTime ) override;
	
	virtual void TakeDamage_Enemy(float DamageAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Plant")
	virtual void OnDeath();

	void InitOnGrid(int32 Row, int32 Col, FVector WorldCenter);
	
	UFUNCTION()
	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};