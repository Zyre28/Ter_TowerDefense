#include "MC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GridManager.h"
#include "InputActionValue.h"
#include "PlantBullet.h"
#include "EngineUtils.h"
#include "PlantProducer.h"

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
	int32 col = 0; int32 row = 0;
	if (GridManager->WorldToGrid(GetActorLocation(), row, col)) {
		UE_LOG(LogTemp, Warning, TEXT("Player is on grid cell [%d,%d]"), row, col);
		GridManager->SpawnPlant(PlantClass, row, col);
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
	case 0:
		SpawnPlantInGrid(APlantProducer::StaticClass());
		break;

	case 1:
		SpawnPlantInGrid(APlantBullet::StaticClass());
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