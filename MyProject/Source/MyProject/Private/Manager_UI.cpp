// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_UI.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "../Widget_Inventory.h"
#include "../Widget_InvenSlot.h"
#include "../MyGameInstance.h"

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

void UManager_UI::PopupUI(UWorld* World, TSubclassOf<UUserWidget> WidgetType)
{
	
	Widgets.Add(CreateWidget(World, Inventory));

	UE_LOG(LogTemp, Warning, TEXT("GameMode CurrentWidget Succeeded!"));
	Widgets.Top()->AddToViewport();


	//auto WInven = Cast<UWidget_Inventory>(Inven);
	//WInven->CreateSlot();
	//WInven->ChangeGold(Gold);
	//bOnInventory = true;
	//
	//if (EquipWeaponId != -1)
	//	WInven->ChangeWeapon(EquipWeaponId, GInstance);
	//
	//if (EquipArmorId != -1)
	//	WInven->ChangeWeapon(EquipArmorId, GInstance);

	World->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void UManager_UI::PopupInventory(UWorld* World, int Gold, int EquipWeaponId, int EquipArmorId, UMyGameInstance* GInstance)
{
	
	PopupUI(World, Inventory);
	auto WInven = Cast<UWidget_Inventory>(Inven);
	WInven->CreateSlot();
	//WInven->ChangeGold(Gold);
	//
	//
	//if (EquipWeaponId != -1)
	//	WInven->ChangeWeapon(EquipWeaponId, GInstance);
	//
	//if (EquipArmorId != -1)
	//	WInven->ChangeWeapon(EquipArmorId, GInstance);
}

void UManager_UI::RemoveUI()
{
}
