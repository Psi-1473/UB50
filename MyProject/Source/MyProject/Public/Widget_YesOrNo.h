// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../DEFINE.H"
#include "Widget_YesOrNo.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_YesOrNo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;


	UFUNCTION()
	void ClickYesButton();
	UFUNCTION()
	void ClickNoButton();

	void TakeQuest();
	void ClearQuest();

	void ChangeText(FString NewText);

public:
	void Init();
	void SetValue(YesOrNoType TypeValue) { MyType = TypeValue; }
	void SetQuestId(int Value) { QuestId = Value; }

private:
	YesOrNoType MyType;
	int QuestId;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Message;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Yes;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_No;

};
