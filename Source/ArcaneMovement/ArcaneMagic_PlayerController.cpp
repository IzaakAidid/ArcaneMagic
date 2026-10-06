// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcaneMagic_PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"

void AArcaneMagic_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	PawnMappingContext = NewObject<UInputMappingContext>(this);
	MoveAction = NewObject<UInputAction>(this);

	MoveAction->ValueType = EInputActionValueType::Axis3D;

}
