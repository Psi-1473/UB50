// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_InvenSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "Components/Image.h"
#include "MyGameInstance.h"


void UWidget_InvenSlot::SetCount(int8 Index)
{
	FString strText;
	SlotIndex = Index;
	//strText = FString::Printf(TEXT("%d"), SlotIndex);
	//Count->SetText(FText::FromString(strText));
	
}

void UWidget_InvenSlot::SetWeaponItem()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());

	if (MyPlayer->WeaponList[SlotIndex] != nullptr)
		ChangeImage(0, MyPlayer->WeaponList[SlotIndex]->Id, GInstance, MyPlayer);
	else
		SetEmpty();
}

void UWidget_InvenSlot::SetArmorItem()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());

	if (MyPlayer->ArmorList[SlotIndex] != nullptr)
		ChangeImage(1, MyPlayer->ArmorList[SlotIndex]->Id, GInstance, MyPlayer);
	else
		SetEmpty();
}

void UWidget_InvenSlot::SetUseItem()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());

	if (MyPlayer->UseItemList[SlotIndex] != nullptr)
		ChangeImage(2, MyPlayer->UseItemList[SlotIndex]->Id, GInstance, MyPlayer);
	else
		SetEmpty();
}

void UWidget_InvenSlot::SetEmpty()
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	FString strText;
	strText = FString::Printf(TEXT(""), 00);
	Count->SetText(FText::FromString(strText));
	Img_Item->SetBrush(GInstance->GetArmorImage(0)->Brush);
}

void UWidget_InvenSlot::ChangeImage(int TypeIndex, int Index, UMyGameInstance* Instance, AMyPlayer* PlayerInst)
{
	FString strText;

	switch (TypeIndex)
	{
	case 0:
		strText = FString::Printf(TEXT("%d"), PlayerInst->WeaponList[SlotIndex]->Id);
		Img_Item->SetBrush(Instance->GetWeaponImage(PlayerInst->WeaponList[SlotIndex]->Id)->Brush);
		break;
	case 1:
		strText = FString::Printf(TEXT("%d"), PlayerInst->ArmorList[SlotIndex]->Id);
		Img_Item->SetBrush(Instance->GetArmorImage(PlayerInst->ArmorList[SlotIndex]->Id)->Brush);
		break;
	case 2:
		strText = FString::Printf(TEXT("%d"), PlayerInst->UseItemList[SlotIndex]->Id);
		Img_Item->SetBrush(Instance->GetUseImage(PlayerInst->UseItemList[SlotIndex]->Id)->Brush);
		break;
	default:
		break;
	}

	Count->SetText(FText::FromString(strText));
}
