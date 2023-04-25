// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Manager_Quest.h"
#include "Manager_Scene.h"
#include "Manager_Inven.h"
#include "Manager_UI.h"
#include "MyPlayer.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Data(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyData(TEXT("DataTable'/Game/Data/EnemyStatTable.EnemyStatTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Weapons(TEXT("DataTable'/Game/Data/WeaponTable.WeaponTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Armors(TEXT("DataTable'/Game/Data/ArmorTable.ArmorTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> UseItems(TEXT("DataTable'/Game/Data/UseItemTable.UseItemTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponImg(TEXT("DataTable'/Game/Data/ImageTable/WeaponImgTable.WeaponImgTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> ArmorImg(TEXT("DataTable'/Game/Data/ImageTable/ArmorImgTable.ArmorImgTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> UseItemImg(TEXT("DataTable'/Game/Data/ImageTable/UseItemImgTable.UseItemImgTable'"));
		

	MyStats = Data.Object;
	EnemyStats = EnemyData.Object;
	WeaponData = Weapons.Object;
	ArmorData = Armors.Object;
	UseItemData = UseItems.Object;
	WeaponImage = WeaponImg.Object;
	ArmorImage = ArmorImg.Object;
	UseItemImage = UseItemImg.Object;

	QuestManager = MakeShared<UManager_Quest>();
	InvenManager = MakeShared<Manager_Inven>();
	UIManager = MakeShared<UManager_UI>();

	SceneManager = NewObject<UManager_Scene>();
	
}
 
void UMyGameInstance::Init()
{
	UGameInstance::Init();
	QuestManager->LoadQuestData();
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

FRichImageRow* UMyGameInstance::GetWeaponImage(int32 Id)
{
	return WeaponImage->FindRow<FRichImageRow>(*FString::FromInt(Id), TEXT(""));
}

FRichImageRow* UMyGameInstance::GetArmorImage(int32 Id)
{
	return ArmorImage->FindRow<FRichImageRow>(*FString::FromInt(Id), TEXT(""));;
}

FRichImageRow* UMyGameInstance::GetUseImage(int32 Id)
{
	return UseItemImage->FindRow<FRichImageRow>(*FString::FromInt(Id), TEXT(""));
}

void UMyGameInstance::SetPlayerData(AMyPlayer* MyPlayer)
{
	//PData.WIndex = MyPlayer->WeaponIndex;
	//PData.AIndex = MyPlayer->ArmorIndex;
	//PData.UIndex = MyPlayer->UseItemIndex;
	//
	//PData.WData.Init(nullptr, 24);
	//PData.AData.Init(nullptr, 24);
	//PData.UData.Init(nullptr, 24);
	//
	//for (int i = 0; i < 24; i++)
	//{
	//	PData.WData[i] = MyPlayer->WeaponList[i];
	//	PData.AData[i] = MyPlayer->ArmorList[i];
	//	PData.UData[i] = MyPlayer->UseItemList[i];
	//}
	//
	//PData.Gold = MyPlayer->Gold;
}
