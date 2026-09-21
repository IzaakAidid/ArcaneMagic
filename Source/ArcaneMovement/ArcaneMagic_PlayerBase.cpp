// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcaneMagic_PlayerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AArcaneMagic_PlayerBase::AArcaneMagic_PlayerBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 0;
	CameraSpringArm->bUsePawnControlRotation = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraSpringArm);
	//we want the camera to use the spring arms rotation, not the pawns
	PlayerCamera->bUsePawnControlRotation = false;

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetupAttachment(PlayerCamera);

}

// Called when the game starts or when spawned
void AArcaneMagic_PlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(GameplayInputContext, 0);
		}
	}
}

// Called every frame
void AArcaneMagic_PlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArcaneMagic_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AArcaneMagic_PlayerBase::PlayerJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Crouch/Slide
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AArcaneMagic_PlayerBase::PlayerCrouch);

		// Sprint
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AArcaneMagic_PlayerBase::PlayerSprint);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AArcaneMagic_PlayerBase::PlayerEndSprint);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AArcaneMagic_PlayerBase::PlayerMove);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AArcaneMagic_PlayerBase::PlayerLook);
	}
}

void AArcaneMagic_PlayerBase::PlayerMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AArcaneMagic_PlayerBase::PlayerLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void AArcaneMagic_PlayerBase::PlayerJump()
{
	Jump();

	//Implement Unique Jump Later
		//Austin is right, if u dont make it, its not yours

	//If sliding, slide jump

	//If on a wall, do a wallkick
}

void AArcaneMagic_PlayerBase::PlayerSprint()
{
	//Increase the players movespeed and FOV bump for immersion
}

void AArcaneMagic_PlayerBase::PlayerEndSprint()
{
	//undo sprint effect and reset player to default values
		//maybe have max stamina
}

void AArcaneMagic_PlayerBase::PlayerCrouch()
{
	// if players speed is fast enough, call slide code instead of crouch code

	// otherwise, shrink the players hitbox and lower the camera 
		//maybe make it a little slower too
}

void AArcaneMagic_PlayerBase::PlayerStopCrouch()
{
	//Undo Crouch Changes
}

void AArcaneMagic_PlayerBase::PlayerSlide()
{
	//slide
}

void AArcaneMagic_PlayerBase::PlayerStopSlide()
{
	//if a jump was called, do nothing i guess

	//if u run out of speed, go into a crouch
}

