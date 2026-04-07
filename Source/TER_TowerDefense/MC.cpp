#include "MC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
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