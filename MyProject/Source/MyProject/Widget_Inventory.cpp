// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Inventory.h"
#include "Components/WrapBox.h"
#include "Widget_InvenSlot.h"

void UWidget_Inventory::NativeOnInitialized()
{
	
}

void UWidget_Inventory::CreateSlot()
{
	for (int i = 0; i < WrapBox_Slot->GetChildrenCount(); i++)
	{
		Slots.Add(Cast<UWidget_InvenSlot>(WrapBox_Slot->GetChildAt(i)));
		Slots[i]->SetCount(i);
	}
	

	
}

void UWidget_Inventory::AddItem()
{
	//Todo
}

void UWidget_Inventory::RemoveItem(int8 Index)
{
	//Todo
}
