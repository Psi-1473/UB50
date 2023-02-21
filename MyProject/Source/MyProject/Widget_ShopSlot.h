// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ShopSlot.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_ShopSlot : public UUserWidget
{
	GENERATED_BODY()
	void InitializeSlot(int ItemId);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Gold;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Buy;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Item;

};
