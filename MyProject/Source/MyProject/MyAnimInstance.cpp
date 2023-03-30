// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyPlayer.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_Attack.AM_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_Damaged.AM_Damaged'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SM(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_Skill.AM_Skill'"));
	if (AM.Succeeded())
		AttackMontage = AM.Object;

	if (DM.Succeeded())
		DamagedMontage = DM.Object;

	if (SM.Succeeded())
		SkillMontage = SM.Object;

}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();

	if (IsValid(pawn))
	{
		Speed = pawn->GetVelocity().Size();

		auto Character = Cast<AMyPlayer>(pawn);

		if (Character)
		{

			if (Speed >= 0.1)
				Character->SetState(MOVING);

			if (Speed <= 0 && Character->GetState() == MOVING)
				Character->SetState(IDLE);

			Horizontal = Character->Horizontal;
			Vertical = Character->Vertical;
			bSprint = Character->bIsSprint;
		}
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.f);
	}
}

void UMyAnimInstance::PlayDamagedMontage()
{
	if (!Montage_IsPlaying(DamagedMontage))
	{
		Montage_Play(DamagedMontage, 1.f);
	}
}

void UMyAnimInstance::PlaySkillMontage(int32 SectionIndex)
{
	if (!Montage_IsPlaying(SkillMontage))
	{
		Montage_Play(SkillMontage, 1.f);
		JumpToSkillSection(SectionIndex);
	}
}

void UMyAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

void UMyAnimInstance::JumpToSkillSection(int32 SectionIndex)
{
	FName Name = GetSkillMontageName(SectionIndex);
	Montage_JumpToSection(Name, SkillMontage);
}

void UMyAnimInstance::AnimNotify_CanCombo()
{
	auto pawn = TryGetPawnOwner();
	if (IsValid(pawn))
	{
		auto Character = Cast<AMyPlayer>(pawn);
		
		if (Character->bCombo == true)
		{
			Character->Attack();
		}
	}
}

void UMyAnimInstance::AnimNotify_EndCombo()
{
	auto pawn = TryGetPawnOwner();
	if (IsValid(pawn))
	{
		auto Character = Cast<AMyPlayer>(pawn);
		Character->EndAttack();
	}
}

void UMyAnimInstance::AnimNotify_HitCheck()
{
	OnAttackHit.Broadcast();
}

void UMyAnimInstance::AnimNotify_HitEnded()
{
	auto MyCharacter = Cast<AMyPlayer>(TryGetPawnOwner());
	MyCharacter->SetState(IDLE);

}

void UMyAnimInstance::AnimNotify_RHitCheck()
{
	auto MyCharacter = Cast<AMyPlayer>(TryGetPawnOwner());

	MyCharacter->SkillRAttackCheck();
}

void UMyAnimInstance::AnimNotify_Fire()
{
	auto MyCharacter = Cast<AMyPlayer>(TryGetPawnOwner());

	MyCharacter->Fire();
}

void UMyAnimInstance::AnimNotify_SkillEnd()
{
	auto MyCharacter = Cast<AMyPlayer>(TryGetPawnOwner());
	MyCharacter->SetState(IDLE);
}

FName UMyAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

FName UMyAnimInstance::GetSkillMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Skill%d"), SectionIndex));
}
