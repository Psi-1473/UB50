// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameInstance.h"
#include "MonsterSpawner.generated.h"

UCLASS()
class MYPROJECT_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterSpawner();

	void CheckMobCount();
	FVector FindSpawnSpot();
	void SpawnMob(FVector Location);



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void DownCount();
	void UpCount();

private:

	MonsterType MobType;
	int MaxMonsterCount;
	int NowMonsterCount = 0;

	float RangeX;
	float RangeY;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;
	FTimerHandle MemberTimerHandle;

	

};
