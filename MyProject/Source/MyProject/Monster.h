// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DEFINE.H"
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

	virtual void OnDamaged();
	virtual void Attack(class AMyPlayer* Target);
	void AttackCheck();
	virtual void Die(class AMyPlayer* Player);
	void DropItemOrGold(class AMyPlayer* Player);
	virtual void OnStun(float Tick);
	void OffStun();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:
	void SetState(STATE Value) { MonsterState = Value; }

	STATE GetState() { return MonsterState; }

	

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
	class AMyPlayer* AttackTarget;

	UPROPERTY()
	bool IsDied = false;

	UPROPERTY()
	class UWidgetComponent* HpBar;

	UPROPERTY()
	float Speed;

	UPROPERTY()
	FTimerHandle StunTimerHandle;
public:
	UPROPERTY()
	bool bStuned;

	STATE MonsterState;

};
