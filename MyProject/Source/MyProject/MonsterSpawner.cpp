// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "NavigationSystem.h"
#include "SpawnMonster.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Check Mob Count!"));
	for (int i = 0; i < MobCountToSpawn; i++)
	{
		FVector SpawnSpot = FindSpawnSpot();
		
		SpawnMob(SpawnSpot);
	}
}

FVector AMonsterSpawner::FindSpawnSpot()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation RandomLocation;

	NavSystem->GetRandomPointInNavigableRadius(SpawnerLocation, 500.f, RandomLocation);
	RandomLocation.Location.Z += 50;
	return RandomLocation.Location;
}

void AMonsterSpawner::SpawnMob(FVector Location)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Mob!"));
	FTransform SpawnTrans;
	SpawnTrans.SetLocation(Location);
	AActor* Mob = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnTrans);
	auto Kwang = Cast<ASpawnMonster>(Mob);
	if (Kwang == nullptr)
		return;
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

