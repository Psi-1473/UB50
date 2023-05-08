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
	void UseItem(UMyGameInstance* GInstance, int Id, int Idx);

	void AddItemWeapon(UMyGameInstance* GInstance, int Id);
	void AddItemArmor(UMyGameInstance* GInstance, int Id);
	void AddItemUse(UMyGameInstance* GInstance, int Id);

	void ChangeGold(UMyGameInstance* GInstance, int Value);

	int FindNextInvenIndex(int ItemType);

	bool DraggingSwap(UMyGameInstance* GInstance, int from, int to);


	void ApplyPotion(int PotionId);

public:
	void SetGold(int Value) { Gold = Value; }
	void SetOnInventory(bool Value) { bOnInventory = Value; }


	TArray<FWeaponData*> GetWeaponList() { return WeaponList; }
	TArray<FArmorData*> GetArmorList() { return ArmorList; }
	TArray<FUseItemData*> GetUseItemList() { return UseItemList; }

	int32 GetWeaponIndex() { return WeaponIndex; }
	int32 GetArmorIndex() { return ArmorIndex; }
	int32 GetUseItemIndex() { return UseItemIndex; }

	int GetEquipWeaponId() { return EquipWeaponId; }
	int GetEquipArmorId() { return EquipArmorId; }
	int GetGold() { return Gold; }
	bool GetOnInventory() { return bOnInventory; }
private:
	TArray<FWeaponData*>	WeaponList;
	TArray<FArmorData*>		ArmorList;
	TArray<FUseItemData*>	UseItemList;

	int32 WeaponIndex = 0;
	int32 ArmorIndex = 0;
	int32 UseItemIndex = 0;

	int Gold = 1000;

	int EquipWeaponId = -1;
	int EquipArmorId = -1;

	bool bOnInventory = false;
};
