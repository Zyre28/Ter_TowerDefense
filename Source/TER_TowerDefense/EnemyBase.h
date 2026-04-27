#pragma once
#include "Components/SphereComponent.h"
#include "EnemyBase.generated.h"

class AMC;
class APlantBase;

DECLARE_DELEGATE(FOnEnemyDeath);

UCLASS(Abstract)
class TER_TOWERDEFENSE_API AEnemyBase : public APawn
{
	GENERATED_BODY()
	
	float TimeRemaining;

	AMC* MC;
	APlantBase* Plant;

protected:
	
	bool bIsAttackingPlant = false;
	bool bIsAttackingMC = false;
	int32 Id;
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats",  meta = (ToolTip = "Time in seconds"))
	float AttackSpeed = 3.f;
	
	// Interval in secondes between each move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float MoveInterval = 0.5f;

	// Move lenght each move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	int32 MoveAmount = 7;
	
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
	
	void SetId(int32 _Rank);
	int32 GetId();

	
	UFUNCTION()
	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	UFUNCTION()
	virtual void EndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
		);
	
	FOnEnemyDeath OnDeathDelegate;
	
	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere)
	USoundBase* GettingHitSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;
	
protected:
		
	float TimeSinceMove = 0.f;

	void Move();
};