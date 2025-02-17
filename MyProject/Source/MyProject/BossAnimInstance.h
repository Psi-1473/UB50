// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBossAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackMontage(int32 SectionIndex);
	void PlayDamagedMontage();
	void PlayDeathMontage();
	void PlayStartMontage();
	FName GetAttackMontageName(int32 SectionIndex);
	void JumpToSection(int32 SectionIndex);

public:
	UFUNCTION()
	void AnimNotify_HitEnded();

	UFUNCTION()
	void AnimNotify_Death();

	UFUNCTION()
	void AnimNotify_Fire();

	UFUNCTION()
	void AnimNotify_Skill2Ready();

	UFUNCTION()
	void AnimNotify_Skill2Fire();

	UFUNCTION()
	void AnimNotify_Skill3Targeting();

	UFUNCTION()
	void AnimNotify_Skill3Fire();

	UFUNCTION()
	void AnimNotify_Skill4Fire();

	UFUNCTION()
	void AnimNotify_CreateProjectile();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool bStuned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* StartMontage;
};
