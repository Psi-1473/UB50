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

	void PopupInventory(UWorld* World, int Gold, int EquipWeaponId, int EquipArmorId, class UMyGameInstance* GInstance);
	void RemoveUI();


	// 인벤토리
	void UpdateInventory(int NextSlot);
	
private:
	void PopupUI(UWorld* World, TSubclassOf<UUserWidget> WidgetType);
private:

	TSubclassOf<UUserWidget> Inventory;
	int InvenNum;
	//UUserWidget* Inven;

	TSubclassOf<UUserWidget> Conversation;
	UUserWidget* Conv;

	TArray<UUserWidget*> Widgets;
};
