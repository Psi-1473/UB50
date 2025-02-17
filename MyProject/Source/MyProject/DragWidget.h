// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UDragWidget : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnyWhere)
	int DragIndex;

	UPROPERTY(EditAnyWhere)
	int DragItemType;

	UPROPERTY(EditAnyWhere)
	int DragItemId;

	UPROPERTY(EditAnyWhere)
	int DragQuickIndex;

	UPROPERTY(EditAnyWhere)
	class UWidget_InvenSlot* DragSlot;
	
	UPROPERTY(EditAnywhere)
	UUserWidget* WidgetReference;

	UPROPERTY(EditAnyWhere)
	FVector2D DragOffset;
};
