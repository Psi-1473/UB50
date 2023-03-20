// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"

UBossAnimInstance::UBossAnimInstance()
{
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();

	if (IsValid(pawn))
	{
		Speed = pawn->GetVelocity().Size();
	}
}
