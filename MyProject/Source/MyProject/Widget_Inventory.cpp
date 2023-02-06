// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Inventory.h"
#include "Components/WrapBox.h"
#include "Widget_InvenSlot.h"

void UWidget_Inventory::NativeOnInitialized()
{
	
}

void UWidget_Inventory::CreateSlot()
{
	auto ItemSlot = Cast<UWidget_InvenSlot>(WrapBox_Slot->GetChildAt(0));
	
	ItemSlot->SetCount();

	
}
