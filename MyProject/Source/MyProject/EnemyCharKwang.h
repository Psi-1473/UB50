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
	void AttackCheck();
	void Die();
	FOnAttackEnd OnAttackEnd;

	void SetSpawner(class AMonsterSpawner* Spawner);


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
public:
	void SetDamaged(bool Value) { IsDamaged = Value; }
	bool GetDamaged() { return IsDamaged; }
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY(VisibleAnywhere)
	class UEnemyStatComponent* Stat;
private:
	UPROPERTY()
	class UEnemyAnimInstance* AnimInst;

	UPROPERTY()
	bool IsDamaged = false;

	UPROPERTY()
	class UWidgetComponent* HpBar;

	UPROPERTY()
	AMonsterSpawner* MySpawner;
};
