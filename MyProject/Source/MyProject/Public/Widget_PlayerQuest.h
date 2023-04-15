// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_PlayerQuest.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_PlayerQuest : public UUserWidget
{
	GENERATED_BODY()
	
/*
	NativeConstruct에서 Player의 StartedQuest 목록을 불러온다.

*/


public:
	UWidget_PlayerQuest(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;

public:
	void ChangeInfo(int QuestId);

	UFUNCTION()
	void RefreshStarted();

	UFUNCTION()
	void RefreshCleared();

	void SlotsMakeEmpty();
	//void AddNowNum();


private:
	void CreateSlot();
	void SetGoalAndNow(int QuestId);



private:
	int ViewQuestId;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Txt_Sub;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Txt_Goal;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Txt_Now;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_Started;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_Completed;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox_List;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BP_Slot;
	TArray<UUserWidget*> Slots;

	bool bStartedQuest = true;
};
