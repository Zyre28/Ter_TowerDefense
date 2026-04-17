#include "MC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GridManager.h"
#include "InputActionValue.h"
#include "PlantBullet.h"
#include "EngineUtils.h"
#include "PlantProducer.h"
#include "UA_Game.h"

AMC::AMC()
{
	PrimaryActorTick.bCanEverTick = true;

	// Movement 
	GetCharacterMovement()->bOrientRotationToMovement = true; // turning while moving
	GetCharacterMovement()->RotationRate  = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed  = 500.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
}

void AMC::BeginPlay()
{
	Super::BeginPlay();
	
	if (UUA_Game* GameInstance = Cast<UUA_Game>(GetGameInstance()))
	{
		GameInstance->SetMC(this);
	}
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Register mapping context Enhanced Input
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		PC->bShowMouseCursor  = true;
		PC->bEnableClickEvents = true;
	}
	if (!GridManager) {
		for (TActorIterator<AGridManager> It(GetWorld()); It; ++It) {
			GridManager = *It;
			UE_LOG(LogTemp, Warning, TEXT("Found GridManager: %s"), *GridManager->GetName());
			break;
		}
	}
	for (TActorIterator<AResourceManager> It(GetWorld()); It; ++It)
	{
		ResourceManager = *It;
		break;
	}
}

void AMC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(ActionDeplacer, ETriggerEvent::Triggered, this, &AMC::Move);
		EIC->BindAction(ActionSauter,   ETriggerEvent::Started,   this, &AMC::Jump_);
		EIC->BindAction(ActionSpawnPlant_1, ETriggerEvent::Started,   this, &AMC::SpawnPlant);
		EIC->BindAction(ActionSpawnPlant_2, ETriggerEvent::Started,   this, &AMC::SpawnPlant);
	}
}

void AMC::Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	if (Controller)
	{
		AddMovementInput(FVector::ForwardVector, Input.Y); // W/S → axis X 
		AddMovementInput(FVector::RightVector,   Input.X); // A/D → axis Y 
	}
}

void AMC::Jump_()
{
	Jump();
}

void AMC::SpawnPlantInGrid(TSubclassOf<class APlantBase> PlantClass) {
	if (!PlantClass || !GridManager || !ResourceManager) return;
	
	const APlantBase* DefaultPlant = PlantClass.GetDefaultObject();
	
	if (!DefaultPlant) return;
	
	int32 col = 0; int32 row = 0;
	if (GridManager->WorldToGrid(GetActorLocation(), row, col)) {
		UE_LOG(LogTemp, Warning, TEXT("Player is on grid cell [%d,%d]"), row, col);
		FGridCell* Cell = GridManager->GetCellFromWorld(GetActorLocation());
		if (!Cell || !Cell->IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Case [%d,%d] déjà occupée"), row, col);
			return;
		}
		if (ResourceManager->SpendResources(DefaultPlant->ActualCost, DefaultPlant->TypeResource))
		{
			GridManager->SpawnPlant(PlantClass, row, col);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You are poor : %d"), ResourceManager->GetResource(DefaultPlant->TypeResource));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Player is NOT on a valid grid cell"));
	}
}

void AMC::SpawnPlant(const FInputActionValue& Value)
{
	float RawValue = Value.Get<float>();
	int32 Key = FMath::RoundToInt(RawValue);

	switch (Key)
	{
	case 1:
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Red,
				FString::Printf(TEXT("Valid plant key (bullet): %d"), Key)
			);
		}
		SpawnPlantInGrid(PlantBullet);
		break;

	case 2:
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Red,
				FString::Printf(TEXT("Valid plant key (producer): %d"), Key)
			);
		}
		SpawnPlantInGrid(PlantProducer);
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid plant key: %d"), Key);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Red,
				FString::Printf(TEXT("Invalid plant key: %d"), Key)
			);
		}
		break;
	}
}

int32 AMC::GetCurrentHealth()
{
	return CurrentHealth;
}

void AMC::TakeDamage_MC(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("MC took : %f damages"), DamageAmount);
	CurrentHealth = (CurrentHealth - (DamageAmount - DamageAmount * Robustness)) > 0 ? (CurrentHealth - (DamageAmount - DamageAmount * Robustness)) : 0;
	if (CurrentHealth == 0)
	{
		OnDeath();
	}
}

void AMC::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("MC died"));
	Destroy();
}