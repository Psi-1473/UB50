// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();

	if (IsValid(pawn))
	{
		Speed = pawn->GetVelocity().Size();
	}
}
