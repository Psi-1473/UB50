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
	UWidget_Shop(const FObjectInitializer &ObjectInitializer);
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void CreateSlot(class ANpc* NPC);
	
	UFUNCTION()
	void CloseUI();

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_List;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;

	TSubclassOf<UUserWidget> BP_Slot;
	UUserWidget* Slot;
};
