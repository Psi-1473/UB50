// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "NavigationSystem.h"
#include "EnemyCharKwang.h"

AMonsterSpawner::AMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	MobType = Kwang;
	MaxMonsterCount = 4;
	RangeX = 1500.f;
	RangeY = 1500.f;


	//ActorToSpawn = 
}

void AMonsterSpawner::CheckMobCount()
{
	int MobCountToSpawn;
	MobCountToSpawn = MaxMonsterCount - NowMonsterCount;

	if (MobCountToSpawn <= 0)
		return;

	for (int i = 0; i < MobCountToSpawn; i++)
	{
		FVector SpawnSpot = FindSpawnSpot();
		
		SpawnMob(SpawnSpot);
	}
}

FVector AMonsterSpawner::FindSpawnSpot()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//if (NavSystem == nullptr)
	//	return;

	FNavLocation RandomLocation;

	//if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, RandomLocation))
	//{
	//	return RandomLocation.Location;
	//}
	//FVector::ZeroVector
	
	NavSystem->GetRandomPointInNavigableRadius(SpawnerLocation, 500.f, RandomLocation);
	return RandomLocation.Location;
}

void AMonsterSpawner::SpawnMob(FVector Location)
{
	FTransform SpawnTrans;
	SpawnTrans.SetLocation(Location);
	AActor* Mob = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnTrans);
	auto Kwang = Cast<AEnemyCharKwang>(Mob);
	Kwang->SetSpawner(this);

}

void AMonsterSpawner::BeginPlay()
{
	AActor::BeginPlay();
	
	SpawnerLocation = GetActorTransform().GetLocation();
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AMonsterSpawner::CheckMobCount, 5.f, true);
}

void AMonsterSpawner::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void AMonsterSpawner::DownCount()
{
	NowMonsterCount--;
}

void AMonsterSpawner::UpCount()
{
	NowMonsterCount++;
}

