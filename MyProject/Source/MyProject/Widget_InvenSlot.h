// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_InvenSlot.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_InvenSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCount(int8 Index);
	void SetWeaponItem();
	void SetArmorItem();
	void SetUseItem();

private:
	UPROPERTY()
	int8 SlotIndex;

	UPROPERTY()
	int8 ItemType;

	UPROPERTY()
	int8 ItemCount;

	UPROPERTY()
	int8 ItemId;

	UPROPERTY()
	FString ItemName;

	UPROPERTY()
	FString ItemSub;

	// 필요하면 나중에 ItemId만 남겨도 될 듯?

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Count;
	
};
