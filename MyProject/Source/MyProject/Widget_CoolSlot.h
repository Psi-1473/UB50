// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_CoolSlot.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_CoolSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void SetId(int Id) { ItemId = Id; }
	void SetIdx(int Idx) {  InvenIndex = Idx; }
	void SetImage();
	void UseItem();

public:
	int QuickId;
	int ItemId = -1;
	int InvenIndex;

private:
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	class UImage* Img_Item;

};