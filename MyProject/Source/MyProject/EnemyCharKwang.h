// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.h"
#include "SpawnMonster.h"
#include "EnemyCharKwang.generated.h"



UCLASS()
class MYPROJECT_API AEnemyCharKwang : public ASpawnMonster
{
	GENERATED_BODY()

public:
	AEnemyCharKwang();



protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
