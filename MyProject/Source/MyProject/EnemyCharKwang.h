// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharKwang.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class MYPROJECT_API AEnemyCharKwang : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharKwang();
	void OnDamaged();
	void Attack();
	FOnAttackEnd OnAttackEnd;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnAttackMontageStarted(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UAnimMontage* AttackMontage;

private:
	UPROPERTY()
	class UEnemyAnimInstance* AnimInst;

};
