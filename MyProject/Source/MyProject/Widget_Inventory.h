// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeOnInitialized();

public:
	void CreateSlot();
	
private:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* WrapBox_Slot;



};
