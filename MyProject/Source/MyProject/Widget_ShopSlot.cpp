// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ShopSlot.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "Components/Image.h"
#include "MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyPlayer.h"



void UWidget_ShopSlot::NativeConstruct()
{
	Btn_Buy->OnClicked.AddDynamic(this, &UWidget_ShopSlot::BuyItem);
}


void UWidget_ShopSlot::InitializeSlot(UMyGameInstance* GInstance, int ItemType, int ItemId)
{
	MyType = ItemType;
	MyId = ItemId;
	switch (ItemType)
	{
	case 0:
		InitWeapon(GInstance, ItemId);
		break;
	case 1:
		InitArmor(GInstance, ItemId);
		break;
	case 2:
		InitUseItem(GInstance, ItemId);
		break;
	default:
		break;
	}
	
}

void UWidget_ShopSlot::InitWeapon(UMyGameInstance* GInstance, int ItemId)
{
	MyPrice = GInstance->GetWeaponData(ItemId)->BuyPrice;
	FString ItemName = GInstance->GetWeaponData(ItemId)->Name;

	FString strText = FString::Printf(TEXT("%d"), MyPrice);
	Txt_Gold->SetText(FText::FromString(strText));

	strText = FString::Printf(TEXT("%s"), *ItemName);
	Txt_Name->SetText(FText::FromString(strText));
	Img_Item->SetBrush(GInstance->GetWeaponImage(ItemId)->Brush);
}

void UWidget_ShopSlot::InitArmor(UMyGameInstance* GInstance, int ItemId)
{
	MyPrice = GInstance->GetArmorData(ItemId)->BuyPrice;
	FString ItemName = GInstance->GetArmorData(ItemId)->Name;

	FString strText = FString::Printf(TEXT("%d"), MyPrice);
	Txt_Gold->SetText(FText::FromString(strText));

	strText = FString::Printf(TEXT("%s"), *ItemName);
	Txt_Name->SetText(FText::FromString(strText));

	Img_Item->SetBrush(GInstance->GetArmorImage(ItemId)->Brush);
}

void UWidget_ShopSlot::InitUseItem(UMyGameInstance* GInstance, int ItemId)
{
	MyPrice = GInstance->GetUseData(ItemId)->BuyPrice;
	FString ItemName = GInstance->GetUseData(ItemId)->Name;

	FString strText = FString::Printf(TEXT("%d"), MyPrice);
	Txt_Gold->SetText(FText::FromString(strText));

	strText = FString::Printf(TEXT("%s"), *ItemName);
	Txt_Name->SetText(FText::FromString(strText));

	Img_Item->SetBrush(GInstance->GetUseImage(ItemId)->Brush);
}

void UWidget_ShopSlot::BuyItem()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	if (MyPlayer->GetGold() < MyPrice)
	{
		UE_LOG(LogTemp, Warning, TEXT("Need More Money"));
		return;
	}

	MyPlayer->SetGold(MyPlayer->GetGold() - MyPrice);

	switch (MyType)
	{
	case WEAPON:
		MyPlayer->AddItemWeapon(MyId);
		break;
	case ARMOR:
		MyPlayer->AddItemArmor(MyId);
		break;
	case USEITEM:
		MyPlayer->AddItemUse(MyId);
		break;
	default:
		break;
	}


}



