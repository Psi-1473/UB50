// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class MYPROJECT_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	AMonster();
	FOnAttackEnd OnAttackEnd;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetSpawner(class AMonsterSpawner* Spawner);
	AMonsterSpawner* GetSpawner() { return MySpawner; }

	void OnDamaged();
	void Attack();
	void AttackCheck();
	void Die();

public:
	void SetDamaged(bool Value) { IsDamaged = Value; }
	bool GetDamaged() { return IsDamaged; }
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnAttackMontageStarted(UAnimMontage* Montage, bool bInterrupted);
public:
	UPROPERTY(VisibleAnywhere)
	class UEnemyStatComponent* Stat;

	UAnimMontage* AttackMontage;
protected:
	UPROPERTY()
	class UEnemyAnimInstance* AnimInst;

	UPROPERTY()
	bool IsDamaged = false;

	UPROPERTY()
	class UWidgetComponent* HpBar;

	UPROPERTY()
	class AMonsterSpawner* MySpawner;

	UPROPERTY()
	float Speed;

};
