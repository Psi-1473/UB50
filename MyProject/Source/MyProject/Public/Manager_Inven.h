// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MyGameInstance.h"

/**
 * 
 */
class MYPROJECT_API Manager_Inven
{
public:
	Manager_Inven();
	~Manager_Inven();

public:
	void EquipWeapon(UMyGameInstance* GInstance, int Id, int Idx);
	void EquipArmor(UMyGameInstance* GInstance, int Id, int Idx);
	void UseItem(class AMyPlayer* MyPlayer, UMyGameInstance* GInstance, int Id, int Idx, int QuickIdx = 0);

	void AddItemWeapon(UMyGameInstance* GInstance, int Id);
	void AddItemArmor(UMyGameInstance* GInstance, int Id);
	void AddItemUse(UMyGameInstance* GInstance, int Id);

	void ChangeGold(UMyGameInstance* GInstance, int Value);

	int FindNextInvenIndex(int ItemType);

	bool DraggingSwap(UMyGameInstance* GInstance, int from, int to);


	void ApplyPotion(class AMyPlayer* MyPlater, int PotionId);

public:
	void SetGold(int Value) { Gold = Value; }
	void SetOnInventory(bool Value) { bOnInventory = Value; }

	TArray<FWeaponData*> GetWeaponList() { return WeaponList; }
	TArray<FArmorData*> GetArmorList() { return ArmorList; }
	TArray<FUseItemData*> GetUseItemList() { return UseItemList; }
	TArray<int> GetUseCount() { return UseCount; }
	int32 GetWeaponIndex() { return WeaponIndex; }
	int32 GetArmorIndex() { return ArmorIndex; }
	int32 GetUseItemIndex() { return UseItemIndex; }

	int GetEquipWeaponId() { return EquipWeaponId; }
	int GetEquipArmorId() { return EquipArmorId; }
	int GetGold() { return Gold; }
	bool GetOnInventory() { return bOnInventory; }
	int CheckUseItemIdx(int Id);
private:
	TArray<FWeaponData*>	WeaponList;
	TArray<FArmorData*>		ArmorList;
	TArray<FUseItemData*>	UseItemList;
	TArray<int> UseCount;
public:
	TArray<int> QuickSlot;

private:

	int32 WeaponIndex = 0;
	int32 ArmorIndex = 0;
	int32 UseItemIndex = 0;

	int Gold = 1000;

	int EquipWeaponId = -1;
	int EquipArmorId = -1;

	bool bOnInventory = false;
};
