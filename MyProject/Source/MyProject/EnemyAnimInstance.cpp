// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Blueprints/Animations/Enemy/AM_KwangAttack.AM_KwangAttack'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();

	if (IsValid(pawn))
	{
		Speed = pawn->GetVelocity().Size();
	}
}

void UEnemyAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.f);
	}
}
