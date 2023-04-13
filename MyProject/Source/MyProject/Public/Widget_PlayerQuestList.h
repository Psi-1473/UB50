// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_PlayerQuestList.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_PlayerQuestList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	void SetParentUI(class UWidget_PlayerQuest* UI) { ParentUI = UI; }
	void SetQuestId(int Id) {
		QuestId = Id;
		ChangeName();
	}

	UFUNCTION()
	void ChangeInfo();

	void ChangeName();
private:
	int QuestId;

	class UWidget_PlayerQuest* ParentUI;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Name;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Quest;
};
