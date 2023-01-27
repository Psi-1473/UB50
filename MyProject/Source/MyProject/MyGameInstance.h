// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MyGameInstance.generated.h"

USTRUCT()
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHp;
};

USTRUCT()
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Speed;
};


/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	virtual void Init() override;

	FMyCharacterData* GetStatData(int32 Level);
	FEnemyData* GetEnemyData(FString Kwang);

private:
	UPROPERTY()
	class UDataTable* MyStats;

	UPROPERTY()
	class UDataTable* EnemyStats;

	
};
