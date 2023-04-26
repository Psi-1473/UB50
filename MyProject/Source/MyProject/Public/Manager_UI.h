// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../DEFINE.H"


/**
 * 
 */
class MYPROJECT_API UManager_UI
{
public:
	UManager_UI();

	UUserWidget* PopupUI(UWorld* World, UIType MyUIType);
	void CloseUI(UIType MyUIType);
	void RefreshUI(); // 나중에 타입 추가하기

	// 인벤토리
	void UpdateInventory(int NextSlot);
	void ChangeInvenGold(int Value);

	void OnConvButton();
	void OffConvButton();
	void ChangeConvLine(FString Line);

private:
	void RemoveUI(int MyUIType);
	void RemoveAllUi();

public:
	UUserWidget* GetInven() { return Widgets[(int)UIType::INVENTORY]; }

private:
	int InvenNum = -1;

	TArray<TSubclassOf<UUserWidget>> WidgetAssets;
	
	TSubclassOf<UUserWidget> YesNo;
	TSubclassOf<UUserWidget> Inventory;
	TSubclassOf<UUserWidget> Conversation;
	TSubclassOf<UUserWidget> ShopUi;
	TSubclassOf<UUserWidget> QuestUi;
	TSubclassOf<UUserWidget> PlayerQuest;
	TSubclassOf<UUserWidget> Investigation;

	TArray<UUserWidget*> Widgets;
};
