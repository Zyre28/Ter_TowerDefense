#include "MC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GenerateManager.h"
#include "GridManager.h"
#include "InputActionValue.h"
#include "EngineUtils.h"
#include "UA_Game.h"
#include "PlantBase.h"
#include "ResourceManager.h"

AMC::AMC()
{
	PrimaryActorTick.bCanEverTick = true;

	// Movement 
	GetCharacterMovement()->bOrientRotationToMovement = true; // turning while moving (still not working)
	GetCharacterMovement()->RotationRate  = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed  = 500.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
}

void AMC::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UUA_Game>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetMC(this);
		PlantSlots.Init(-1, GameInstance->MaxSlot);
		//AssignPlantToSlot(0, 0); // Debugg only
		//AssignPlantToSlot(1, 1); 
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
	if (!GenerateManager) {
		for (TActorIterator<AGridManager> It(GetWorld()); It; ++It) {
			GenerateManager = *It;
			UE_LOG(LogTemp, Warning, TEXT("Found GridManager: %s"), *GenerateManager->GetName());
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
		EIC->BindAction(ActionSauter, ETriggerEvent::Started, this, &AMC::Jump_);

		if (ActionSpawnPlant_1)
			EIC->BindAction(ActionSpawnPlant_1, ETriggerEvent::Started, this, &AMC::SpawnPlantSlot0);
		if (ActionSpawnPlant_2)
			EIC->BindAction(ActionSpawnPlant_2, ETriggerEvent::Started, this, &AMC::SpawnPlantSlot1);
		if (ActionSpawnPlant_3)
			EIC->BindAction(ActionSpawnPlant_3, ETriggerEvent::Started, this, &AMC::SpawnPlantSlot2);
		if (ActionSpawnPlant_4)
			EIC->BindAction(ActionSpawnPlant_4, ETriggerEvent::Started, this, &AMC::SpawnPlantSlot3);
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
	if (!PlantClass || !GenerateManager || !ResourceManager) return;
	
	AGridManager* GridManager = Cast<AGridManager>(GenerateManager);
	if (!GridManager) return;
	
	const APlantBase* DefaultPlant = PlantClass.GetDefaultObject();
	
	if (!DefaultPlant) return;
	
	int32 col = 0; int32 row = 0;
	if (GridManager->WorldToGrid(GetActorLocation(), row, col)) {
		UE_LOG(LogTemp, Warning, TEXT("Player is on grid cell [%d,%d]"), row, col);
		FGridCell* Cell = GridManager->GetCellFromWorld(GetActorLocation());
		if (!Cell || !Cell->IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Case [%d,%d] occupied"), row, col);
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

int32 AMC::GetCurrentHealth()
{
	return CurrentHealth;
}

void AMC::TakeDamage_MC(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("MC took : %f damages"), DamageAmount);
	float health = CurrentHealth - (DamageAmount - DamageAmount * Robustness);
	CurrentHealth = health > 0 ? health : 0;
	if (CurrentHealth == 0)
	{
		OnDeath();
	}
}

void AMC::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("MC died"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			FString::Printf(TEXT("MC died"))
		);
	}
	bIsDead = true;
}

void AMC::AssignPlantToSlot(int32 SlotIndex, int32 PlantIndex)
{
	if (!PlantSlots.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SlotIndex %d invalid"), SlotIndex);
		return;
	}
	if (!AvailablePlants.IsValidIndex(PlantIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlantIndex %d invalid"), PlantIndex);
		return;
	}
	if (GameInstance)
	{
		FString PlantName = AvailablePlants[PlantIndex]->GetName();
		//UE_LOG(LogTemp, Warning, TEXT(" UE name : %s"), *PlantName);
		FPlantData* Data = GameInstance->PlantsData.Find(PlantName);
		if (!Data)
		{
			UE_LOG(LogTemp, Error, TEXT("Plant not found: %s"), *PlantName);
			return;
		}

		if (Data->Availability != 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s not equipped"), *PlantName);
			return;
		}
	}

	PlantSlots[SlotIndex] = PlantIndex;
	UE_LOG(LogTemp, Warning, TEXT("Slot %d → %s"), SlotIndex, *AvailablePlants[PlantIndex]->GetName());
}

void AMC::ClearSlot(int32 SlotIndex)
{
	if (!PlantSlots.IsValidIndex(SlotIndex))
	{
		return;
	}
	PlantSlots[SlotIndex] = -1;
}

void AMC::SpawnPlantAtSlot(int32 SlotIndex)
{
	if (!PlantSlots.IsValidIndex(SlotIndex))
	{
		return;
	}

	int32 PlantIndex = PlantSlots[SlotIndex];
	if (PlantIndex == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot %d si empty"), SlotIndex);
		return;
	}

	if (!AvailablePlants.IsValidIndex(PlantIndex))
	{
		return;
	}

	SpawnPlantInGrid(AvailablePlants[PlantIndex]);
}

TObjectPtr<AGenerateManager> AMC::GetGenerateManager()
{
	return GenerateManager;
}

void AMC::SetCurrentHealth(int32 _CurrentHealth)
{
	CurrentHealth = _CurrentHealth;
}

void AMC::RefreshSlotsFromGameInstance()
{
	if (!GameInstance) return;
    
	// Vide tous les slots
	for (int32 i = 0; i < GameInstance->MaxSlot; i++)
	{
		ClearSlot(i);
	}
    
	// Remplit avec les plantes qui ont Availability == 2
	int32 SlotIndex = 0;
	for (const auto& Pair : GameInstance->PlantsData)
	{
		if (Pair.Value.Availability == 2 && SlotIndex < GameInstance->MaxSlot)
		{
			int32 PlantIndex = -1;
			for (int32 i = 0; i < AvailablePlants.Num(); i++)
			{
				if (AvailablePlants[i] && AvailablePlants[i]->GetName() == Pair.Key)
				{
					PlantIndex = i;
					break;
				}
			}
            
			if (PlantIndex != -1)
			{
				// Appelle AssignPlantToSlot SANS la vérification d'Availability
				// ou modifie AssignPlantToSlot pour ne plus vérifier
				PlantSlots[SlotIndex] = PlantIndex;
				SlotIndex++;
			}
		}
	}
}

AMC::~AMC()
{
	if (GameInstance && IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMC destruction"));
		GameInstance->UnSetMC();	
	}
}