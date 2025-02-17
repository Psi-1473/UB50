// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Inventory.h"
#include "Components/WrapBox.h"
#include "Widget_InvenSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DragWidget.h"
#include "MyGameInstance.h"
#include "Manager_Inven.h"
#include "Components/Image.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"



void UWidget_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Weapon->OnClicked.AddDynamic(this, &UWidget_Inventory::RefreshToWeapon);
	Btn_Armor->OnClicked.AddDynamic(this, &UWidget_Inventory::RefreshToArmor);
	Btn_Use->OnClicked.AddDynamic(this, &UWidget_Inventory::RefreshToUse);
	Btn_Exit->OnClicked.AddDynamic(this, &UWidget_Inventory::CloseUI);

	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UseGInstance
	OwnerPlayer = MyPlayer;
	CreateSlot();
	ChangeGold(GInstance->InvenManager->GetGold());
	ChangeWeapon(GInstance->InvenManager->GetEquipWeaponId(), GInstance);
	ChangeArmor(GInstance->InvenManager->GetEquipArmorId(), GInstance);
	
}

bool UWidget_Inventory::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}

void UWidget_Inventory::CreateSlot()
{
	for (int i = 0; i < WrapBox_Slot->GetChildrenCount(); i++)
	{
		Slots.Add(Cast<UWidget_InvenSlot>(WrapBox_Slot->GetChildAt(i)));
		Slots[i]->SetCount(i);
	}	
	RefreshSlot(TypeIndex);
}

void UWidget_Inventory::AddItem()
{
	//Todo
}

void UWidget_Inventory::RemoveItem(int8 Index)
{
	//Todo
}

void UWidget_Inventory::RefreshSlot(int8 ItemType)
{
	switch (ItemType)
	{
	case WEAPON:
		TypeIndex = -1;
		RefreshToWeapon();
		break;
	case ARMOR:
		TypeIndex = -1;
		RefreshToArmor();
		break;
	case USEITEM:
		TypeIndex = -1;
		RefreshToUse();
		break;
	default:
		break;
	}
}

void UWidget_Inventory::RefreshToWeapon()
{
	if (TypeIndex == 0)
		return;

	TypeIndex = 0;

	for (int i = 0; i < WrapBox_Slot->GetChildrenCount(); i++)
	{
		Slots[i]->SetWeaponItem();
	}
}
void UWidget_Inventory::RefreshToArmor()
{
	if (TypeIndex == 1)
		return;
	TypeIndex = 1;

	for (int i = 0; i < WrapBox_Slot->GetChildrenCount(); i++)
	{
		Slots[i]->SetArmorItem();
	}
}
void UWidget_Inventory::RefreshToUse()
{
	if (TypeIndex == 2)
		return;
	TypeIndex = 2;

	for (int i = 0; i < WrapBox_Slot->GetChildrenCount(); i++)
	{
		Slots[i]->SetUseItem();
	}
}

void UWidget_Inventory::CloseUI()
{
	UseGInstance
	OwnerPlayer->CloseUI(UIType::INVENTORY);
	GInstance->InvenManager->SetOnInventory(false);
}

void UWidget_Inventory::ChangeGold(int Value)
{
	FString strText;
	Gold = Value;
	strText = FString::Printf(TEXT("%d"), Gold);
	Txt_Gold->SetText(FText::FromString(strText));
}

void UWidget_Inventory::ChangeWeapon(int Id, UMyGameInstance* GInstance)
{
	if (Id == -1)
		return;
	Img_EquipWeapon->Brush = GInstance->GetWeaponImage(Id)->Brush;
}

void UWidget_Inventory::ChangeArmor(int Id, UMyGameInstance* GInstance)
{
	if (Id == -1)
		return;
	Img_EquipArmor->Brush = GInstance->GetArmorImage(Id)->Brush;
}



void UWidget_Inventory::SetInvenIndex(int ItemType, int Value)
{
	switch (ItemType)
	{
	case WEAPON:
		WeaponIndex = Value;
		break;
	case ARMOR:
		ArmorIndex = Value;
		break;
	case USEITEM:
		UseIndex = Value;
		break;
	}
}


