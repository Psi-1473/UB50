// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_UI.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "../Widget_Inventory.h"
#include "Widget_Conversation.h"
#include "../Widget_InvenSlot.h"
#include "Widget_Popup.h"
#include "Widget_Quest.h"
#include "../MyGameInstance.h"
#include "../Define.h"

UManager_UI::UManager_UI()
{
	WidgetAssets.Init(nullptr, WIDGETCOUNT);
	Widgets.Init(nullptr, WIDGETCOUNT);
	static ConstructorHelpers::FClassFinder<UUserWidget> INVEN(TEXT("WidgetBlueprint'/Game/UI/WBP_Inventory.WBP_Inventory_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> CONV(TEXT("WidgetBlueprint'/Game/UI/WBP_Conversation.WBP_Conversation_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> SHOP(TEXT("WidgetBlueprint'/Game/UI/WBP_Shop.WBP_Shop_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> QUEST(TEXT("WidgetBlueprint'/Game/UI/WBP_Quest.WBP_Quest_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> YESNO(TEXT("WidgetBlueprint'/Game/UI/WBP_YesOrNo.WBP_YesOrNo_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> PLAYERQUEST(TEXT("WidgetBlueprint'/Game/UI/WBP_PlayerQuest.WBP_PlayerQuest_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> INVEST(TEXT("WidgetBlueprint'/Game/UI/WBP_Investigation.WBP_Investigation_C'"));

	if (INVEN.Succeeded())
		WidgetAssets[(int)UIType::INVENTORY] = INVEN.Class;

	if (CONV.Succeeded())
		WidgetAssets[(int)UIType::CONVERSATION] = CONV.Class;
	
	if (SHOP.Succeeded())
		WidgetAssets[(int)UIType::SHOP] = SHOP.Class;

	if (QUEST.Succeeded())
		WidgetAssets[(int)UIType::QUEST] = QUEST.Class;

	if (YESNO.Succeeded())
		WidgetAssets[(int)UIType::YESNO] = YESNO.Class;

	if (PLAYERQUEST.Succeeded())
		WidgetAssets[(int)UIType::PLAYERQUEST] = PLAYERQUEST.Class;

	if (INVEST.Succeeded())
		WidgetAssets[(int)UIType::INVESTIGATION] = INVEST.Class;
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
	auto Inven = Widgets[(int)UIType::INVENTORY];
	auto MyInven = Cast<UWidget_Inventory>(Inven);

	if (MyInven == nullptr)
		return;

	MyInven->ChangeGold(Value);
}

void UManager_UI::OnConvButton()
{
	if (Widgets[(int)UIType::CONVERSATION] == nullptr)
		return;

	auto Conv = Cast<UWidget_Conversation>(Widgets[(int)UIType::CONVERSATION]);
	Conv->SetButtonOnOff(ESlateVisibility::Visible);
}

void UManager_UI::OffConvButton()
{
	if (Widgets[(int)UIType::CONVERSATION] == nullptr)
		return;

	auto Conv = Cast<UWidget_Conversation>(Widgets[(int)UIType::CONVERSATION]);
	Conv->SetButtonOnOff(ESlateVisibility::Hidden);
}

void UManager_UI::ChangeConvLine(FString Line)
{
	if (Widgets[(int)UIType::CONVERSATION] == nullptr)
		return;

	auto Conv = Cast<UWidget_Conversation>(Widgets[(int)UIType::CONVERSATION]);
	Conv->ChangeLine(Line);
}

UUserWidget* UManager_UI::PopupUI(UWorld* World, UIType MyUIType)
{
	if (Widgets[(int)MyUIType] != nullptr)
		return nullptr;

	UUserWidget* PopupUi;
	PopupUi = CreateWidget(World, WidgetAssets[(int)MyUIType]);
	PopupUi->AddToViewport();
	Widgets[(int)MyUIType] = PopupUi;
	return PopupUi;
}


void UManager_UI::CloseUI(UIType MyUIType)
{
	if (MyUIType == UIType::ALL)
	{
		RemoveAllUi();
		return;
	}

	if (Widgets[(int)MyUIType] == nullptr)
		return;

	RemoveUI((int)MyUIType);
}

void UManager_UI::RefreshUI()
{
	if (Widgets[(int)UIType::QUEST] == nullptr)
		return;

	Cast<UWidget_Quest>(Widgets[(int)UIType::QUEST])->Refresh();
}

void UManager_UI::RemoveUI(int MyUIType)
{
	if (Widgets[MyUIType] == nullptr)
		return;

	Widgets[MyUIType]->RemoveFromViewport();
	Widgets[MyUIType] = nullptr;
}

void UManager_UI::RemoveAllUi()
{
	for (int i = 0; i < WIDGETCOUNT; i++)
	{
		if(Widgets[i] == nullptr)
			continue;

		RemoveUI(i);
	}
}

