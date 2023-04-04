// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "BossMonster.h"

UBossAnimInstance::UBossAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Blueprints/Animations/Enemy/AM_BossSkills.AM_BossSkills'"));

	if (AM.Succeeded())
		AttackMontage = AM.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM(TEXT("AnimMontage'/Game/Blueprints/Animations/Enemy/AM_BossDamaged.AM_BossDamaged'"));

	if (AM.Succeeded())
		AttackMontage = AM.Object;
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

void UBossAnimInstance::PlayAttackMontage(int32 SectionIndex)
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.f);
		JumpToSection(SectionIndex);
	}
}

void UBossAnimInstance::PlayDamagedMontage()
{
}

void UBossAnimInstance::PlayDeathMontage()
{
}

FName UBossAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	if(SectionIndex == 0)//∆Ú≈∏
		return FName(*FString::Printf(TEXT("Attack")));

	return FName(*FString::Printf(TEXT("Skill%d"), SectionIndex));
}

void UBossAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

void UBossAnimInstance::AnimNotify_HitEnded()
{
	auto pawn = TryGetPawnOwner();
	auto Boss = Cast<ABossMonster>(pawn);

	Boss->SetState(IDLE);
}

void UBossAnimInstance::AnimNotify_Fire()
{
	auto pawn = TryGetPawnOwner();
	auto Boss = Cast<ABossMonster>(pawn);

	Boss->Skill1Fire();
}

void UBossAnimInstance::AnimNotify_Skill2Ready()
{
	auto pawn = TryGetPawnOwner();
	auto Boss = Cast<ABossMonster>(pawn);

	Boss->Skill2Ready();
}

void UBossAnimInstance::AnimNotify_Skill2Fire()
{
	auto pawn = TryGetPawnOwner();
	auto Boss = Cast<ABossMonster>(pawn);

	Boss->Skill2Fire();
}

void UBossAnimInstance::AnimNotify_Skill3Fire()
{
	auto pawn = TryGetPawnOwner();
	auto Boss = Cast<ABossMonster>(pawn);

	Boss->Skill3Targeting();
}

void UBossAnimInstance::AnimNotify_Skill4Fire()
{
	auto pawn = TryGetPawnOwner();
	auto Boss = Cast<ABossMonster>(pawn);

	Boss->Skill4Fire();
}

void UBossAnimInstance::AnimNotify_CreateProjectile()
{
	auto pawn = TryGetPawnOwner();
	auto Boss = Cast<ABossMonster>(pawn);

	Boss->Skill4Charge();
}
