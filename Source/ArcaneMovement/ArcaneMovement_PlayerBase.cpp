// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcaneMovement_PlayerBase.h"

// Sets default values
AArcaneMovement_PlayerBase::AArcaneMovement_PlayerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArcaneMovement_PlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArcaneMovement_PlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArcaneMovement_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

