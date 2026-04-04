// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MC.generated.h"

UCLASS()
class TER_TOWERDEFENSE_API AMC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	// --- Input ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<class UInputAction> ActionDeplacer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<class UInputAction> ActionSauter;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private :
	void Deplacer(const struct FInputActionValue& Value);
	void Sauter();
};
