// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FonAttackHit);

UCLASS()
class MYPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackMontage();
	void PlayDamagedMontage();
	void PlayDeathMontage();

public:
	UFUNCTION()
	void AnimNotify_HitEnded();

	UFUNCTION()
	void AnimNotify_HitCheck();

	UFUNCTION()
	void AnimNotify_Death();

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

public:
	FonAttackHit OnAttackHit;
	
};
