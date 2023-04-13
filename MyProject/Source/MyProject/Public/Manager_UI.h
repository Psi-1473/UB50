// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../DEFINE.H"
#include "Manager_UI.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UManager_UI : public UObject
{
	GENERATED_BODY()
	
public:
	UManager_UI();

	UUserWidget* PopupUI(UWorld* World, UIType MyUIType);
	void CloseUI(UIType MyUIType);


	// 인벤토리
	void UpdateInventory(int NextSlot);
	void ChangeInvenGold(int Value);

private:
	void RemoveUI(UUserWidget* Widget);
	
public:
	UUserWidget* GetInven() { return Inven; }

private:

	TSubclassOf<UUserWidget> YesNo;
	UUserWidget* UIYesNo;

	TSubclassOf<UUserWidget> Inventory;
	UUserWidget* Inven;
	int InvenNum;

	TSubclassOf<UUserWidget> Conversation;
	UUserWidget* Conv;

	TSubclassOf<UUserWidget> ShopUi;
	UUserWidget* Shop;

	TSubclassOf<UUserWidget> QuestUi;
	UUserWidget* UIQuest;

	TSubclassOf<UUserWidget> PlayerQuest;
	UUserWidget* UIPlayerQuest;

	TArray<UUserWidget*> Widgets;
};
