// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_CoolSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture.h"



void UWidget_CoolSlot::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void UWidget_CoolSlot::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
	UUserWidget::NativeTick(Geometry, DeltaSeconds);

	if (CoolTime <= 0)
		return;

	
}

void UWidget_CoolSlot::InitImage(UImage* Img)
{
	Img_Skill->Brush = Img->Brush;
	CoolTime = 0;
}

void UWidget_CoolSlot::ChangeCoolTime()
{
	CoolTime--;

	FString strText;

	if (CoolTime <= 0)
	{
		CoolTime = 0;
		strText = FString::Printf(TEXT(""));
	}
	else
		strText = FString::Printf(TEXT("%d"), CoolTime);
	
	
	Txt_Cooltime->SetText(FText::FromString(strText));
}



