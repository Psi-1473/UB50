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

	void SetCount(int8 Index);
	void SetWeaponItem();
	void SetArmorItem();
	void SetUseItem();

	void SetEmpty();
	void ChangeImage(int TypeIndex, int Index, class UMyGameInstance* Instance, class AMyPlayer* PlayerInst);
	//0 ���� 1 �� 2 �Ҹ�ǰ

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

	

	// �ʿ��ϸ� ���߿� ItemId�� ���ܵ� �� ��?

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Count;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Item;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Slot;
	
};
