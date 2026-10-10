// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ArcaneMagic_PlayerMovementComp.generated.h"

/**
 *
 */
UCLASS(meta = (BlueprintSpawnableComponent, DisplayName = "BaseMovementComponent"))
class ARCANEMOVEMENT_API UArcaneMagic_PlayerMovementComp : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickTyp, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundMovement")
	float MaxWalkSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundMovement")
	float Acceleration = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundMovement")
	float MaxStepHeight = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundMovement")
	float MaxSlopAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundMovement")
	float WalkableFloorZ = 0.71f;

};
