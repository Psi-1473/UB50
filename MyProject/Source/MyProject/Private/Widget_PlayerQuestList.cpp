// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayerQuestList.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Widget_PlayerQuest.h"
#include "../MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Manager_Quest.h"

void UWidget_PlayerQuestList::NativeConstruct()
{
	Btn_Quest->OnClicked.AddDynamic(this, &UWidget_PlayerQuestList::ChangeInfo);
}

void UWidget_PlayerQuestList::SetQuestId(AMyGameMode* GMode, int Id)
{
		QuestId = Id;
		UE_LOG(LogTemp, Warning, TEXT("Set Quest Id : %d"), QuestId);
		QuestName = GMode->QuestManager->GetQuestName(QuestId);
		Txt_Name->SetText(FText::FromString(QuestName));

		if (GMode->QuestManager->GetStartedQuestById(QuestId).CanClear)
			return;

		Img_Clear->SetVisibility(ESlateVisibility::Hidden);
		//ChangeName(GMode);
}

void UWidget_PlayerQuestList::ChangeInfo()
{
	ParentUI->ChangeInfo(QuestId);
}

void UWidget_PlayerQuestList::ChangeName(AMyGameMode* GMode)
{
	
}
