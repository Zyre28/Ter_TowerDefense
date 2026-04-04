// Fill out your copyright notice in the Description page of Project Settings.


#include "MC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
// Sets default values
AMC::AMC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// --- Mouvement ---
	GetCharacterMovement()->bOrientRotationToMovement = true; // tourne vers la direction
	GetCharacterMovement()->RotationRate  = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed  = 500.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
}

// Called when the game starts or when spawned
void AMC::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Enregistre le mapping context Enhanced Input
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		// Curseur visible (utile en top-down)
		PC->bShowMouseCursor  = true;
		PC->bEnableClickEvents = true;
	}
}

// Called every frame
void AMC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(ActionDeplacer, ETriggerEvent::Triggered, this, &AMC::Deplacer);
		EIC->BindAction(ActionSauter,   ETriggerEvent::Started,   this, &AMC::Sauter);
	}
}

void AMC::Deplacer(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	if (Controller)
	{
		AddMovementInput(FVector::ForwardVector, Input.Y); // W/S → axe X monde
		AddMovementInput(FVector::RightVector,   Input.X); // A/D → axe Y monde
	}
}

void AMC::Sauter()
{
	Jump();
}