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
	static ConstructorHelpers::FClassFinder<UUserWidget> CW(TEXT("WidgetBlueprint'/Game/UI/WBP_Conversation.WBP_Conversation_C'"));
	if (CW.Succeeded())
	{
		Conversation = CW.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> SW(TEXT("WidgetBlueprint'/Game/UI/WBP_Shop.WBP_Shop_C'"));
	if (SW.Succeeded())
	{
		ShopUi = SW.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> QW(TEXT("WidgetBlueprint'/Game/UI/WBP_Quest.WBP_Quest_C'"));
	if (QW.Succeeded())
	{
		QuestUi = QW.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> YW(TEXT("WidgetBlueprint'/Game/UI/WBP_YesOrNo.WBP_YesOrNo_C'"));
	if (YW.Succeeded())
	{
		YesNo = YW.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> PQW(TEXT("WidgetBlueprint'/Game/UI/WBP_PlayerQuest.WBP_PlayerQuest_C'"));
	if (PQW.Succeeded())
	{
		PlayerQuest = PQW.Class;
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

UUserWidget* UManager_UI::PopupUI(UWorld* World, UIType MyUIType)
{
	switch (MyUIType)
	{
	case UIType::YESNO:
		UIYesNo = CreateWidget(World, YesNo);
		UIYesNo->AddToViewport();
		return UIYesNo;
		break;
	case UIType::INVENTORY:
		Inven = CreateWidget(World, Inventory);
		Inven->AddToViewport();
		return Inven;
		break;
	case UIType::CONVERSATION:
		Conv = CreateWidget(World, Conversation);
		Conv->AddToViewport();
		return Conv;
		break;
	case UIType::SHOP:
		Shop = CreateWidget(World, ShopUi);
		Shop->AddToViewport();
		return Shop;
		break;
	case UIType::QUEST:
		UIQuest = CreateWidget(World, QuestUi);
		UIQuest->AddToViewport();
		return UIQuest;
		break;
	case UIType::PLAYERQUEST:
		if (UIPlayerQuest != nullptr)
		{
			CloseUI(UIType::PLAYERQUEST);
			UE_LOG(LogTemp, Log, TEXT("Player Quest UI Close!"));
			UIPlayerQuest = nullptr;
			World->GetFirstPlayerController()->SetShowMouseCursor(false);
			return nullptr;
		}
		else
		{
			UIPlayerQuest = CreateWidget(World, PlayerQuest);
			UIPlayerQuest->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("Player Quest UI Open!"));
			return UIQuest;
		}
		break;
	default:
		return nullptr;
		break;
	}
	
	//World->GetFirstPlayerController()->SetShowMouseCursor(true);
}


void UManager_UI::CloseUI(UIType MyUIType)
{
	switch (MyUIType)
	{
	case UIType::YESNO:
		RemoveUI(UIYesNo);
		break;
	case UIType::INVENTORY:
		RemoveUI(Inven);
		break;
	case UIType::CONVERSATION:
		RemoveUI(Conv);
		break;
	case UIType::SHOP:
		if (Shop == nullptr) return;
		RemoveUI(Shop);
		break;
	case UIType::QUEST:
		if (UIQuest == nullptr) return;
		RemoveUI(UIQuest);
	case UIType::PLAYERQUEST:
		RemoveUI(UIPlayerQuest);
	default:
		break;
	}
}

void UManager_UI::RemoveUI(UUserWidget* Widget)
{
	if (Widget == nullptr)
		return;

	Widget->RemoveFromViewport();
	Widget = nullptr;
}
