// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyCharKwang.h"



UEnemyAnimInstance::UEnemyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Blueprints/Animations/Enemy/AM_KwangAttack.AM_KwangAttack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM(TEXT("AnimMontage'/Game/Blueprints/Animations/Enemy/AM_KwangDamaged.AM_KwangDamaged'"));
	if (AM.Succeeded())
		AttackMontage = AM.Object;

	if (DM.Succeeded())
		DamagedMontage = DM.Object;
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

void UEnemyAnimInstance::PlayDamagedMontage()
{
	if (!Montage_IsPlaying(DamagedMontage))
	{
		Montage_Play(DamagedMontage, 1.f);
	}
}

void UEnemyAnimInstance::AnimNotify_HitEnded()
{
	auto pawn = TryGetPawnOwner();

	AEnemyCharKwang* Pawn = Cast<AEnemyCharKwang>(pawn);
	Pawn->SetDamaged(false);
}

void UEnemyAnimInstance::AnimNotify_HitCheck()
{
	OnAttackHit.Broadcast();
}


