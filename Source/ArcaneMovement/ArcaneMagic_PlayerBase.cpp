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

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetupAttachment(RootComponent);


	// Create a CameraComponent	
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 0;
	CameraSpringArm->bUsePawnControlRotation = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraSpringArm);
	//we want the camera to use the spring arms rotation, not the pawns
	PlayerCamera->bUsePawnControlRotation = false;

	WallKickDetectionRange = 60;


	playerIsSliding = false;

	playerCanWallKick = false;
	walkKickDirection = FVector::Zero();

	playerCanJump = true;
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

	if (GetMovementComponent()->IsFalling())
	{
		playerCanWallKick = true;
		CheckForWallCollision();
	}
	else
	{
		playerCanJump = true;
		playerCanWallKick = false;
		walkKickDirection = FVector::Zero();
	}
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

		// Crouch/Slide
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AArcaneMagic_PlayerBase::PlayerSlide);

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
		AddMovementInput(GetActorForwardVector() * MovementVector.Y, true);
		AddMovementInput(GetActorRightVector() * MovementVector.X, true);
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
	//Implement Unique Jump Later
		//Austin is right, if u dont make it, its not yours

	if (GetCharacterMovement()->IsFalling() && playerCanWallKick && walkKickDirection != FVector::Zero())
	{
		FVector impulseDirection = FVector(walkKickDirection.X * 450, walkKickDirection.Y * 350, 200);
		GetCharacterMovement()->AddImpulse(impulseDirection,  /*bVelChange=*/true);
		playerCanJump = true;
	}

	if (playerCanJump)
	{
		if (!playerIsSliding)
		{
			FVector impulseDirection = FVector(GetActorForwardVector().X * 100, GetActorForwardVector().Y * 100, 500);
			GetCharacterMovement()->AddImpulse(impulseDirection,  /*bVelChange=*/true);
			playerCanJump = false;
		}
		//If sliding, slide jump
		else
		{
			FVector impulseDirection = FVector(GetActorForwardVector().X * 100, GetActorForwardVector().Y * 100, 500);
			GetCharacterMovement()->AddImpulse(impulseDirection,  /*bVelChange=*/true);
			playerCanJump = false;
		}
	}

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

void AArcaneMagic_PlayerBase::CheckForWallCollision()
{
	//Setup the hit result, aka where the info is being held ( we only care bout the normals of the objects
	FCollisionQueryParams TraceParams(FName(TEXT("InteractionTrace")), true, this);

	// Ignore the character casting the ray
	TraceParams.AddIgnoredActor(this);


	//Setup raytrace start and end pos ( aka the dirrection its going and how far out it goes)

	FVector traceStartPOS = GetActorLocation();

	FHitResult LeftHitResult; //the left raytrace check

	FVector LeftAngle = FVector(GetActorForwardVector() + GetActorRightVector()); //angle in which we'll send the player for left connection
	FVector leftTraceEndPOS = traceStartPOS + (GetActorRightVector() * WallKickDetectionRange);
	bool leftHit = GetWorld()->LineTraceSingleByChannel(
		LeftHitResult,           // Struct to store hit data
		traceStartPOS,       // Start of the raytrace
		leftTraceEndPOS,         // End of the raytrace
		ECC_WorldStatic,     // Collision channel (checks objects static objects / map geometry ) (in theory)
		TraceParams          // rn its just ignoring the player in the raytrace but can be changed to be more precise
	);

	//DrawDebugLine(GetWorld(), traceStartPOS, leftTraceEndPOS, leftHit ? FColor::Green : FColor::Red, false, 0.1f, 0, 1.0f);


	FHitResult RightHitResult;//the right raytrace check

	FVector RightAngle = FVector(GetActorForwardVector() - GetActorRightVector());
	FVector rightTraceEndPOS = traceStartPOS + (-GetActorRightVector() * WallKickDetectionRange);
	bool rightHit = GetWorld()->LineTraceSingleByChannel(
		RightHitResult,           // Struct to store hit data
		traceStartPOS,       // Start of the raytrace
		rightTraceEndPOS,         // End of the raytrace
		ECC_WorldStatic,     // Collision channel (checks objects static objects / map geometry ) (in theory)
		TraceParams          // rn its just ignoring the player in the raytrace but can be changed to be more precise
	);

	//DrawDebugLine(GetWorld(), traceStartPOS, rightTraceEndPOS, rightHit ? FColor::Green : FColor::Red, false, 0.1f, 0, 1.0f);

	//If you hit something of interest
	if (leftHit || rightHit)
	{
		if (leftHit)
		{
			//using the actors forward direction, reflect it off the wall using the wall's normal and then use that as the "angle" for the wall kick
			FVector Reflected = FMath::GetReflectionVector(LeftAngle, LeftHitResult.ImpactNormal);
			walkKickDirection = Reflected;
			playerCanWallKick = true;

			//DrawDebugLine(GetWorld(), LeftHitResult.ImpactPoint, LeftHitResult.ImpactPoint + (Reflected * WallKickDetectionRange), FColor::Green, false, 0.1f, 0, 1.0f);
		}
		if (rightHit)
		{
			//using the actors forward direction, reflect it off the wall using the wall's normal and then use that as the "angle" for the wall kick
			FVector Reflected = FMath::GetReflectionVector(RightAngle, RightHitResult.ImpactNormal);
			walkKickDirection = Reflected;
			playerCanWallKick = true;

			//DrawDebugLine(GetWorld(), RightHitResult.ImpactPoint, RightHitResult.ImpactPoint + (Reflected * WallKickDetectionRange), FColor::Green, false, 0.1f, 0, 1.0f);
		}
	}
	else
	{
		playerCanWallKick = false;
		walkKickDirection = FVector::Zero();
	}

}
