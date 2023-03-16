// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnMonster.h"
#include "MonsterSpawner.h"

void ASpawnMonster::SetSpawner(AMonsterSpawner* Spawner)
{
	MySpawner = Spawner;
	MySpawner->UpCount();
}

void ASpawnMonster::Die(AMyPlayer* Player)
{
	Super::Die(Player);
	MySpawner->DownCount();
}
