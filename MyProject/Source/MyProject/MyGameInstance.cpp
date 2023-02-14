// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Data(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyData(TEXT("DataTable'/Game/Data/EnemyStatTable.EnemyStatTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Weapons(TEXT("DataTable'/Game/Data/WeaponTable.WeaponTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Armors(TEXT("DataTable'/Game/Data/ArmorTable.ArmorTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> UseItems(TEXT("DataTable'/Game/Data/UseItemTable.UseItemTable'"));
		
	MyStats = Data.Object;
	EnemyStats = EnemyData.Object;
	WeaponData = Weapons.Object;
	ArmorData = Armors.Object;
	UseItemData = UseItems.Object;

	//for (int i = 1; i < 4; i++)
	//{
	//	static ConstructorHelpers::FObjectFinder<UImage> WImage(TEXT("Texture2D'/Game/Graphics/Items/Weapons/Sword1.Sword1'"));
	//	//WeaponImage.Add(i, WImage.Object);
	//}
}

void UMyGameInstance::Init()
{
	UGameInstance::Init();
	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %d"), GetStatData(1)->Attack);

	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %s"), *GetEnemyData(TEXT("Kwang"))->Name);
}


FMyCharacterData* UMyGameInstance::GetStatData(int32 Level)
{
	return MyStats->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}

FEnemyData* UMyGameInstance::GetEnemyData(FString Kwang)
{
	return EnemyStats->FindRow<FEnemyData>(*Kwang, TEXT(""));
}

FWeaponData* UMyGameInstance::GetWeaponData(int32 Id)
{
	return WeaponData->FindRow<FWeaponData>(*FString::FromInt(Id), TEXT(""));
}

FArmorData* UMyGameInstance::GetArmorData(int32 Id)
{
	return ArmorData->FindRow<FArmorData>(*FString::FromInt(Id), TEXT(""));
}

FUseItemData* UMyGameInstance::GetUseData(int32 Id)
{
	return UseItemData->FindRow<FUseItemData>(*FString::FromInt(Id), TEXT(""));
}
