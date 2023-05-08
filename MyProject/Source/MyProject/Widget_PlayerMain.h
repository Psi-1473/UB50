// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_PlayerMain.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_PlayerMain : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHp(float Percentage);

	void ChangeQText(int Value);
	void ChangeRText(int Value);
	void ChangeEText(int Value);

	void SetImageAlpha(int Type, bool On);

	void ClickQuickSlot(int Number);

private:

	// 텍스트 이름 확인
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Q;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_R;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_E;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Q;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_R;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_E;
	

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_Hp;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_Mp;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_Exp;


	UPROPERTY(meta = (BindWidget), EditAnywhere)
	class UWidget_CoolSlot* QuickItem;
	
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	class UWidget_CoolSlot* QuickItem2;
	
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	class UWidget_CoolSlot* QuickItem3;
	
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	class UWidget_CoolSlot* QuickItem4;
	
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	class UWidget_CoolSlot* QuickItem5;
	
};
