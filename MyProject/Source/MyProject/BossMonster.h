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

	void SetAttacking(bool Value) { IsAttacking = Value; }



protected:
	UPROPERTY()
	class UBossAnimInstance* AnimInst;

private:
	float ActionCoolTime;
	float SkillActionCoolTime;
	TArray<float> SkillCooltimes;

	bool CanBeStuned;
	bool CanBeStiffed;
	bool IsAttacking;
	UPROPERTY()
	class AMyPlayer* AttackTarget;


	
};
