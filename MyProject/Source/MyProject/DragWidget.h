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

	UPROPERTY(EditAnywhere)
	UUserWidget* WidgetReference;

	UPROPERTY(EditAnyWhere)
	FVector2D DragOffset;
};
