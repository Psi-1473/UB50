// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_InvenSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"

void UWidget_InvenSlot::SetCount(int8 Index)
{
	FString strText;
	SlotIndex = Index;
	strText = FString::Printf(TEXT("%d"), SlotIndex);
	Count->SetText(FText::FromString(strText));
}

void UWidget_InvenSlot::SetWeaponItem()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	if (MyPlayer->WeaponList[SlotIndex] != nullptr)
	{
		FString strText;
		strText = FString::Printf(TEXT("%d"), MyPlayer->WeaponList[SlotIndex]->Attack);
		Count->SetText(FText::FromString(strText));
	}
}
