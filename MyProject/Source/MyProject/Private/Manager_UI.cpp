// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_UI.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "../Widget_Inventory.h"
#include "../Widget_InvenSlot.h"
#include "../MyGameInstance.h"
#include "../Define.h"

UManager_UI::UManager_UI()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> IW(TEXT("WidgetBlueprint'/Game/UI/WBP_Inventory.WBP_Inventory_C'"));
	if (IW.Succeeded())
	{
		Inventory = IW.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> CW(TEXT("WidgetBlueprint'/Game/UI/WBP_Shop.WBP_Shop_C'"));
	if (IW.Succeeded())
	{
		Conversation = CW.Class;
	}
}

void UManager_UI::UpdateInventory(int NextSlot)
{
	if (InvenNum == -1)
		return;

	auto MyInven = Cast<UWidget_Inventory>(Widgets[InvenNum]);

	switch (MyInven->GetInvenType())
	{
	case WEAPON:
		MyInven->Slots[MyInven->GetWeaponIndex()]->SetWeaponItem();
		MyInven->SetInvenIndex(WEAPON, NextSlot);
		UE_LOG(LogTemp, Warning, TEXT("NextSlot -> %d"), MyInven->GetWeaponIndex());
		break;
	case ARMOR:
		MyInven->Slots[MyInven->GetArmorIndex()]->SetArmorItem();
		MyInven->SetInvenIndex(ARMOR, NextSlot);
		break;
	case USEITEM:
		MyInven->Slots[MyInven->GetUseIndex()]->SetUseItem();
		MyInven->SetInvenIndex(USEITEM, NextSlot);
		break;
	}
}

void UManager_UI::ChangeInvenGold(int Value)
{
	auto MyInven = Cast<UWidget_Inventory>(Inven);

	if (MyInven == nullptr)
		return;

	MyInven->ChangeGold(Value);
}

void UManager_UI::PopupUI(UWorld* World, int UIType)
{
	switch (UIType)
	{
	case INVENTORY:
		Inven = CreateWidget(World, Inventory);
		Inven->AddToViewport();
		break;
	case CONVERSATION:
		Conv = CreateWidget(World, Conversation);
		Conv->AddToViewport();
		break;
	}
	
	//World->GetFirstPlayerController()->SetShowMouseCursor(true);
}


void UManager_UI::CloseUI(int UIType)
{
	switch (UIType)
	{
	case INVENTORY:
		RemoveUI(Inven);
		break;
	case CONVERSATION:
		RemoveUI(Conv);
		break;
	default:
		break;
	}
}

void UManager_UI::RemoveUI(UUserWidget* Widget)
{
	if (Widget == nullptr)
		return;
	Widget->RemoveFromViewport();
}
