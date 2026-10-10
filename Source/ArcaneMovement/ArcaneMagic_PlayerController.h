// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcaneMagic_PlayerController.generated.h"

/**
 *
 */
UCLASS()
class ARCANEMOVEMENT_API AArcaneMagic_PlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void SetupInputComponent() override;

	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;

	UPROPERTY()
	class UInputAction* MoveAction;
};
