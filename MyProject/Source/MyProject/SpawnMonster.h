// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "SpawnMonster.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASpawnMonster : public AMonster
{
	GENERATED_BODY()
	

public:
	ASpawnMonster();
	
	virtual void PostInitializeComponents() override;
	void SetSpawner(class AMonsterSpawner* Spawner);
	AMonsterSpawner* GetSpawner() { return MySpawner; }

	virtual void OnDamaged() override;
	virtual void Attack(class AMyPlayer* Target) override;
	virtual void Die(class AMyPlayer* Player) override;
	virtual void OnStun(float Tick) override;

protected:
	UPROPERTY()
	class UEnemyAnimInstance* AnimInst;

	UPROPERTY()
	class AMonsterSpawner* MySpawner;
};
