// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Inven.h"
#include "../Define.h"
#include "../Widget_Inventory.h"
#include "../Widget_InvenSlot.h"
#include "../MyGameInstance.h"
#include "Manager_UI.h"
#include "Manager_Quest.h"
#include "../MyPlayer.h"


Manager_Inven::Manager_Inven()
{
	WeaponList.Init(nullptr, INVENTORYNUMBER);
	ArmorList.Init(nullptr, INVENTORYNUMBER);
	UseItemList.Init(nullptr, INVENTORYNUMBER);

	UseCount.Init(0, 24);
	QuickSlot.Init(-1, 24);
}

Manager_Inven::~Manager_Inven()
{
}

void Manager_Inven::EquipWeapon(UMyGameInstance* GInstance, int Id, int Idx)
{
	auto MyInven = Cast<UWidget_Inventory>(GInstance->UIManager->GetInven());
	if (MyInven == nullptr)
		return;
	if (GInstance->GetWeaponData(Id) == nullptr)
		return;

	if (EquipWeaponId != -1)
		WeaponList[Idx] = GInstance->GetWeaponData(EquipWeaponId);
	else
		WeaponList[Idx] = nullptr;

	MyInven->Slots[Idx]->SetWeaponItem();
	MyInven->ChangeWeapon(Id, GInstance);
	EquipWeaponId = Id;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Weapon Equip!"));
}

void Manager_Inven::EquipArmor(UMyGameInstance* GInstance, int Id, int Idx)
{
	auto MyInven = Cast<UWidget_Inventory>(GInstance->UIManager->GetInven());

	if (GInstance->GetArmorData(Id) == nullptr)
		return;

	if (EquipArmorId != -1)
		ArmorList[Idx] = GInstance->GetArmorData(EquipArmorId);
	else
		ArmorList[Idx] = nullptr;

	MyInven->Slots[Idx]->SetArmorItem();
	MyInven->ChangeArmor(Id, GInstance);
	EquipArmorId = Id;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Armor Equip!"));
}

void Manager_Inven::UseItem(AMyPlayer* MyPlayer, UMyGameInstance* GInstance, int Id, int Idx, int QuickIdx)
{
	if (GInstance->GetUseData(Id) == nullptr)
		return;

	if (UseCount[Idx] <= 0)
		return;
	 
	UseCount[Idx]--;

	if (UseCount[Idx] <= 0)
	{
		UseCount[Idx] = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Use Item Count 0!"));
		UseItemList[Idx] = nullptr;
		
	}

	auto MyInven = Cast<UWidget_Inventory>(GInstance->UIManager->GetInven());
	if (MyInven != nullptr)
	{
		MyInven->Slots[Idx]->SetUseItem();
	}
	ApplyPotion(MyPlayer, Id);

	// 아이템 정보를 받아와서 
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("item Use!"));
}


void Manager_Inven::AddItemWeapon(UMyGameInstance* GInstance, int Id)
{
	int Idx = FindNextInvenIndex(WEAPON);
	WeaponList[Idx] = GInstance->GetWeaponData(Id);
	if (bOnInventory)
		GInstance->UIManager->UpdateInventory(FindNextInvenIndex(WEAPON));
}

void Manager_Inven::AddItemArmor(UMyGameInstance* GInstance, int Id)
{
	int Idx = FindNextInvenIndex(ARMOR);
	ArmorList[Idx] = GInstance->GetArmorData(Id);
	if (bOnInventory)
		GInstance->UIManager->UpdateInventory(FindNextInvenIndex(ARMOR));
}

void Manager_Inven::AddItemUse(UMyGameInstance* GInstance, int Id)
{
	int Num = CheckUseItemIdx(Id);
	if (Num != -1)
	{
		UseCount[Num]++;
		return;
	}

	int Idx = FindNextInvenIndex(USEITEM);
	UseItemList[Idx] = GInstance->GetUseData(Id);
	UseCount[Idx]++;
	if (bOnInventory)
		GInstance->UIManager->UpdateInventory(FindNextInvenIndex(USEITEM));

	GInstance->QuestManager->AddQuestTargetNum("Collect", Id);
}

void Manager_Inven::ChangeGold(UMyGameInstance* GInstance, int Value)
{
	Gold += Value;

	if (bOnInventory)
	{
		GInstance->UIManager->ChangeInvenGold(Value);
	}
}

int Manager_Inven::FindNextInvenIndex(int ItemType)
{
	// 인벤토리가 가득 찼을 때 처리.
	for (int i = 0; i < INVENTORYNUMBER; i++)
	{
		switch (ItemType)
		{
		case WEAPON:
			if (WeaponList[i] == nullptr)
				return i;
			break;
		case ARMOR:
			if (ArmorList[i] == nullptr)
				return i;
			break;
		case USEITEM:
			if (UseItemList[i] == nullptr)
				return i;
			break;
		}
	}

	return -1;
}

bool Manager_Inven::DraggingSwap(UMyGameInstance* GInstance, int from, int to)
{
	auto MyInven = Cast<UWidget_Inventory>(GInstance->UIManager->GetInven());
	int FromId;
	int ToId;
	int FromCount;
	int ToCount;

	switch (MyInven->GetInvenType())
	{
	case WEAPON:
		FromId = WeaponList[from]->Id;

		if (WeaponList[to] == nullptr)
		{
			WeaponList[from] = nullptr;
			WeaponIndex = from;
		}
		else
		{
			ToId = WeaponList[to]->Id;
			WeaponList[from] = GInstance->GetWeaponData(ToId);
		}
		WeaponList[to] = GInstance->GetWeaponData(FromId);
		MyInven->Slots[to]->SetWeaponItem();
		MyInven->Slots[from]->SetWeaponItem();

		break;
	case ARMOR:
		FromId = ArmorList[from]->Id;
		if (ArmorList[to] == nullptr)
		{
			ArmorList[from] = nullptr;
			ArmorIndex = from;
		}
		else
		{
			ToId = ArmorList[to]->Id;
			ArmorList[from] = GInstance->GetArmorData(ToId);
		}
		ArmorList[to] = GInstance->GetArmorData(FromId);
		MyInven->Slots[from]->SetArmorItem();
		MyInven->Slots[to]->SetArmorItem();
		break;
	case USEITEM:
		FromId = UseItemList[from]->Id;
		FromCount = UseCount[from];
		ToCount = UseCount[to];
		if (UseItemList[to] == nullptr)
		{
			UseItemList[from] = nullptr;
			UseCount[from] = 0;
			UseItemIndex = from;
		}
		else
		{
			ToId = UseItemList[to]->Id;
			UseCount[from] = ToCount;
			UseItemList[from] = GInstance->GetUseData(ToId);
		}
		UseItemList[to] = GInstance->GetUseData(FromId);
		UseCount[to] = FromCount;
		MyInven->Slots[from]->SetUseItem();
		MyInven->Slots[to]->SetUseItem();
		break;
	}

	return true;
}

void Manager_Inven::ApplyPotion(AMyPlayer* MyPlayer, int PotionId)
{

	switch (PotionId)
	{
	case 1:
		MyPlayer->Recovery(30);//30은 임시
		break;
	default:
		break;
	}
}


int Manager_Inven::CheckUseItemIdx(int Id)
{
	for (int i = 0; i < UseItemList.Num(); i++)
	{
		if(UseItemList[i] == nullptr)
			continue;

		if (UseItemList[i]->Id != Id)
			continue;

		if (UseCount[i] >= 10)
			continue;;

		return i;
	}
	return -1;
}


