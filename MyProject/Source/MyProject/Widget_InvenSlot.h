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
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void SetCount(int8 Index);
	void SetWeaponItem();
	void SetArmorItem();
	void SetUseItem();

	void SetEmpty();
	void RefreshSlot(UWidget_InvenSlot* FromSlot);
	void ChangeImage(int TypeIndex, int Index, class UMyGameInstance* Instance, class AMyPlayer* PlayerInst);
	//0 무기 1 방어구 2 소모품

	UFUNCTION()
	void ClickSlot();
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
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidget_InvenSlot> DragVisualClass;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Count;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Item;

	
};
