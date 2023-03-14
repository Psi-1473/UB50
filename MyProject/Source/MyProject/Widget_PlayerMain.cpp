// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayerMain.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"


void UWidget_PlayerMain::UpdateHp(float Percentage)
{
	PB_Hp->SetPercent(Percentage);
}

void UWidget_PlayerMain::ChangeQText(int Value)
{
	FString strText;
	if(Value == 0)
		strText = FString::Printf(TEXT(""));
	else
		strText = FString::Printf(TEXT("%d"), Value);

	Txt_Q->SetText(FText::FromString(strText));

}

void UWidget_PlayerMain::ChangeRText(int Value)
{
	FString strText;
	if (Value == 0)
		strText = FString::Printf(TEXT(""));
	else
		strText = FString::Printf(TEXT("%d"), Value);

	Txt_R->SetText(FText::FromString(strText));
}

void UWidget_PlayerMain::ChangeEText(int Value)
{
	FString strText;
	if (Value == 0)
		strText = FString::Printf(TEXT(""));
	else
		strText = FString::Printf(TEXT("%d"), Value);

	Txt_E->SetText(FText::FromString(strText));
}

