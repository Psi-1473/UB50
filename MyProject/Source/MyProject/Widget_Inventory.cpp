// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Inventory.h"
#include "Components/WrapBox.h"
#include "Widget_InvenSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DragWidget.h"

void UWidget_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Weapon->OnClicked.AddDynamic(this, &UWidget_Inventory::RefreshToWeapon);
	Btn_Armor->OnClicked.AddDynamic(this, &UWidget_Inventory::RefreshToArmor);
	Btn_Use->OnClicked.AddDynamic(this, &UWidget_Inventory::RefreshToUse);
}

bool UWidget_Inventory::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragWidget* DragWidgetResult = Cast<UDragWidget>(InOperation);

	if (!IsValid(DragWidgetResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast returned null"));
		return false;
	}

	const FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	const FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;

	DragWidgetResult->WidgetReference->AddToViewport();
	DragWidgetResult->WidgetReference->SetVisibility(ESlateVisibility::Visible);
	DragWidgetResult->WidgetReference->SetPositionInViewport(DragWindowOffsetResult, false);
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
	case 0:
		TypeIndex = -1;
		RefreshToWeapon();
		break;
	case 1:
		TypeIndex = -1;
		RefreshToArmor();
		break;
	case 2:
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

void UWidget_Inventory::ChangeGold(int Value)
{
	FString strText;
	Gold = Value;
	strText = FString::Printf(TEXT("%d"), Gold);
	Txt_Gold->SetText(FText::FromString(strText));
}


