// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyPlayer.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_Attack.AM_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_Damaged.AM_Damaged'"));
	if (AM.Succeeded())
		AttackMontage = AM.Object;

	if (DM.Succeeded())
		DamagedMontage = DM.Object;

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

void UMyAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
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
	MyCharacter->SetDamaged(false);

}

FName UMyAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}
