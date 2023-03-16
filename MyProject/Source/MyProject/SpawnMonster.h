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

	virtual void Die(class AMyPlayer* Player) override;
private:
	UPROPERTY()
	class AMonsterSpawner* MySpawner;
};
