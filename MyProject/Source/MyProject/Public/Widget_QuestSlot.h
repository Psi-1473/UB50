// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_QuestSlot.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_QuestSlot : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	void SetId(int Id);
	void SetClear() { bCleared = true; }

private:
	void Init();

	UFUNCTION()
	void PopupYesOrNo();
private:
	int QuestId;
	bool bCleared;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Sub;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Quest;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Clear;
};
