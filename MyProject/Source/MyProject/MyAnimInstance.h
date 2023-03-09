// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FonAttackHit);
/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void PlayAttackMontage();
	void PlayDamagedMontage();
	void PlaySkillMontage();
	void JumpToSection(int32 SectionIndex);

	UFUNCTION()
	void AnimNotify_CanCombo();

	UFUNCTION()
	void AnimNotify_EndCombo();

	UFUNCTION()
	void AnimNotify_HitCheck();

	UFUNCTION()
	void AnimNotify_HitEnded();

	UFUNCTION()
	void AnimNotify_RHitCheck();

	FName GetAttackMontageName(int32 SectionIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bSprint;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillMontage;

public:
	FonAttackHit OnAttackHit;
};
