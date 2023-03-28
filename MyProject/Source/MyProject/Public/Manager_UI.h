// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

	void PopupUI(UWorld* World, int UIType);
	void CloseUI(int UIType);


	// 인벤토리
	void UpdateInventory(int NextSlot);
	void ChangeInvenGold(int Value);

private:
	void RemoveUI(UUserWidget* Widget);
	
public:
	UUserWidget* GetInven() { return Inven; }

private:

	TSubclassOf<UUserWidget> Inventory;
	int InvenNum;
	UUserWidget* Inven;

	TSubclassOf<UUserWidget> Conversation;
	UUserWidget* Conv;

	TArray<UUserWidget*> Widgets;
};
