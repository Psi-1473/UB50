// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "BossMonster.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ABossMonster : public AMonster
{
	GENERATED_BODY()
	
public:
	ABossMonster();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnDamaged() override;
	virtual void Attack(class AMyPlayer* Target) override;
	//virtual void Die(class AMyPlayer* Player) override;

public:
	bool CanUseSkill();
	int PickUsableSkill();
	
	void UseSkill(int SkillNum);

	void Skill1();
	void Skill2();
	void Skill3();
	void Skill4();

	void Skill1Fire();

	void Skill2Ready();
	void Skill2Fire();

	void Skill3Targeting();
	void Skill3Fire(FVector Transform);

	void Skill4Charge();
	void Skill4Fire();

	void SetAttacking(bool Value) { IsAttacking = Value; }

	void SkillCoolTimeZero(BossSkill SkillType);

	void CoolTimeZero1();
	void CoolTimeZero2();
	void CoolTimeZero3();
	void CoolTimeZero4();
	void ActionCoolTimeZero();


private:
	class AProjectile* CreateProjectile(TSubclassOf<class AProjectile> ClassOfProjectile, FVector Muzzle, FRotator Rot);


protected:
	UPROPERTY()
	class UBossAnimInstance* AnimInst;

private:
	float SkillActionCoolTime;
	TArray<bool> UsableSkills;

	bool CanBeStuned;
	bool CanBeStiffed;

public:
	UPROPERTY(EditAnywhere)
	bool IsAttacking;
private:
	bool CanSkills = true;

	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere)
	FVector MuzzleLocation4;

	TArray<AProjectile*> Projectiles;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> Skill4ProjectileClass;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Skill2Emitter;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Skill3Emitter;

	UPROPERTY()
	FTimerHandle SkillTimerHandle;

	UPROPERTY()
	FTimerHandle Skill1TimerHandle;
	UPROPERTY()
	FTimerHandle Skill2TimerHandle;
	UPROPERTY()
	FTimerHandle Skill3TimerHandle;
	UPROPERTY()
	FTimerHandle Skill4TimerHandle;

	UPROPERTY()
	FTimerHandle ProjectileTimer;

	
};
