// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_InvenSlot.h"
#include "Components/TextBlock.h"

void UWidget_InvenSlot::SetCount(int8 Index)
{
	FString strText;
	SlotIndex = Index;
	strText = FString::Printf(TEXT("%d"), SlotIndex);
	Count->SetText(FText::FromString(strText));
}
