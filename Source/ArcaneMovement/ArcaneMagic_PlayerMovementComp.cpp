// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcaneMagic_PlayerMovementComp.h"
#include "Components/CapsuleComponent.h"

void UArcaneMagic_PlayerMovementComp::TickComponent(float DeltaTime, ELevelTick TickTyp, FActorComponentTickFunction* ThisTickFunction)
{
	ConsumeInputVector();
}
