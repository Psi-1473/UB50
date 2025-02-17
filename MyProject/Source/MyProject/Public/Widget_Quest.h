// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Npc.h"
#include "Widget_Quest.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_Quest : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWidget_Quest(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;


public:
	void SetNpcId();
	void CreateQuestList();
	void Refresh();

private:
	int NpcId;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_ListBack;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BP_Slot;
	TArray<UUserWidget*> Slots;

};
