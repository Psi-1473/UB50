// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_InvenSlot.h"
#include "Components/TextBlock.h"

void UWidget_InvenSlot::SetCount()
{
	FString strText;
	strText = FString::Printf(TEXT("%d"), 12);
	Count->SetText(FText::FromString(strText));
}
