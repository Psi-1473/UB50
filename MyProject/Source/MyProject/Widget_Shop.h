// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Shop.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_Shop : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void CreateSlot();
	

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_List;

	TSubclassOf<UUserWidget> BP_Slot;
	UUserWidget* Slot;
};
