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

USTRUCT()
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	FWeaponData()
	{
		Name = "None";
		Id = -1;
		Attack = 0;
		SellPrice = 0;
		BuyPrice = 0;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BuyPrice;
};

USTRUCT()
struct FArmorData : public FTableRowBase
{
	GENERATED_BODY()

	FArmorData()
	{
		Name = "None";
		Id = -1;
		Defense = 0;
		SellPrice = 0;
		BuyPrice = 0;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BuyPrice;
};

USTRUCT()
struct FUseItemData : public FTableRowBase
{
	GENERATED_BODY()
		
	FUseItemData()
	{
		Name = "None";
		Id = -1;
		Hp = 0;
		Mp = 0;
		SellPrice = 0;
		BuyPrice = 0;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Mp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BuyPrice;
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
	FWeaponData* GetWeaponData(int32 Id);
	FArmorData* GetArmorData(int32 Id);
	FUseItemData* GetUseData(int32 Id);

private:
	UPROPERTY()
	class UDataTable* MyStats;

	UPROPERTY()
	class UDataTable* EnemyStats;

	UPROPERTY()
	class UDataTable* WeaponData;

	UPROPERTY()
	class UDataTable* ArmorData;

	UPROPERTY()
	class UDataTable* UseItemData;

	UPROPERTY()
	TMap<int, class UImage*> WeaponImage;

	UPROPERTY()
	TMap<int, class UImage*> ArmorImage;

	UPROPERTY()
	TMap<int, class UImage*> UseItemImage;

	
};
