// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayerMain.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Widget_CoolSlot.h"
#include "DEFINE.H"


void UWidget_PlayerMain::UpdateHp(float Percentage)
{
	PB_Hp->SetPercent(Percentage);
}

void UWidget_PlayerMain::ChangeQText(int Value)
{
	FString strText;
	if (Value == 0)
	{
		strText = FString::Printf(TEXT(""));
		SetImageAlpha(PLAYERSKILL_R, false);
	}
	else
		strText = FString::Printf(TEXT("%d"), Value);

	Txt_Q->SetText(FText::FromString(strText));

}

void UWidget_PlayerMain::ChangeRText(int Value)
{
	FString strText;
	if (Value == 0)
	{
		strText = FString::Printf(TEXT(""));
		SetImageAlpha(PLAYERSKILL_Q, false);
	}
	else
		strText = FString::Printf(TEXT("%d"), Value);

	Txt_R->SetText(FText::FromString(strText));
}

void UWidget_PlayerMain::ChangeEText(int Value)
{
	FString strText;
	if (Value == 0)
	{
		strText = FString::Printf(TEXT(""));
		SetImageAlpha(PLAYERSKILL_E, false);
	}
	else
		strText = FString::Printf(TEXT("%d"), Value);

	Txt_E->SetText(FText::FromString(strText));
}

void UWidget_PlayerMain::SetImageAlpha(int Type, bool On)
{
	FLinearColor NewColor(1.f, 1.f, 1.f, 1.f);

	if (On == true)
		NewColor.A = 0.5f;
	
	if(On == false)
		NewColor.A = 1.f;

	switch (Type)
	{
	case 0:
		Img_Q->SetColorAndOpacity(NewColor);
		break;
	case 1:
		Img_R->SetColorAndOpacity(NewColor);
		break;
	case 2:
		Img_E->SetColorAndOpacity(NewColor);
		break;
	}
	
}

void UWidget_PlayerMain::ClickQuickSlot(int Number)
{
	switch (Number)
	{
	case 1:
		QuickItem->UseItem();
		break;
	case 2:
		QuickItem2->UseItem();
		break;
	case 3:
		QuickItem3->UseItem();
		break;
	case 4:
		QuickItem4->UseItem();
		break;
	case 5:
		QuickItem5->UseItem();
		break;
	default:
		break;
	}
}

void UWidget_PlayerMain::ChangeQuickIndex(int Number, int Idx)
{
	switch (Number)
	{
	case 1:
		QuickItem->SetIdx(Idx);
		break;
	case 2:
		QuickItem2->SetIdx(Idx);
		break;
	case 3:
		QuickItem3->SetIdx(Idx);
		break;
	case 4:
		QuickItem4->SetIdx(Idx);
		break;
	case 5:
		QuickItem5->SetIdx(Idx);
		break;
	default:
		break;
	}
}

void UWidget_PlayerMain::SetQuickId()
{
	QuickItem->QuickId = 1;
	QuickItem2->QuickId = 2;
	QuickItem3->QuickId = 3;
	QuickItem4->QuickId = 4;
	QuickItem5->QuickId = 5;
}

void UWidget_PlayerMain::SaveQuickSlot()
{
	UseGInstance
	GInstance->QuickedId[0] = QuickItem->GetItemId();
	GInstance->QuickedId[1] = QuickItem2->GetItemId();
	GInstance->QuickedId[2] = QuickItem3->GetItemId();
	GInstance->QuickedId[3] = QuickItem4->GetItemId();
	GInstance->QuickedId[4] = QuickItem5->GetItemId();

}

void UWidget_PlayerMain::LoadQuickSlot()
{
	UseGInstance

	QuickItem->SetId(GInstance->QuickedId[0]);
	QuickItem2->SetId(GInstance->QuickedId[1]);
	QuickItem3->SetId(GInstance->QuickedId[2]);
	QuickItem4->SetId(GInstance->QuickedId[3]);
	QuickItem5->SetId(GInstance->QuickedId[4]);

	if (QuickItem->GetItemId() != -1) QuickItem->SetImage();
	if (QuickItem2->GetItemId() != -1) QuickItem2->SetImage();
	if (QuickItem3->GetItemId() != -1) QuickItem3->SetImage();
	if (QuickItem4->GetItemId() != -1) QuickItem4->SetImage();
	if (QuickItem5->GetItemId() != -1) QuickItem5->SetImage();
}

