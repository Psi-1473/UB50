// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayerQuest.h"
#include "../MyGameMode.h"
#include "Manager_Quest.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Widget_PlayerQuestList.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UWidget_PlayerQuest::UWidget_PlayerQuest(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> QUESTLIST(TEXT("WidgetBlueprint'/Game/UI/PlayerQuestList.PlayerQuestList_C'"));
	if (QUESTLIST.Succeeded())
	{
		BP_Slot = QUESTLIST.Class;
	}
}

void UWidget_PlayerQuest::NativeConstruct()
{
	CreateSlot();

	Btn_Started->OnClicked.AddDynamic(this, &UWidget_PlayerQuest::RefreshStarted);
	Btn_Completed->OnClicked.AddDynamic(this, &UWidget_PlayerQuest::RefreshCleared);
}

void UWidget_PlayerQuest::ChangeInfo(int QuestId)
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	FString Type = GameMode->QuestManager->GetQuest(QuestId).Type;
	FString Sub = GameMode->QuestManager->GetQuest(QuestId).Sub;

	Txt_Sub->SetText(FText::FromString(Sub));

	int Id;
	if (Type == "Normal")
		Id = -1;
	else
		Id = QuestId;

	SetGoalAndNow(Id);
}

void UWidget_PlayerQuest::RefreshStarted()
{
	if (bStartedQuest)
		return;

	UseGameMode
	SlotsMakeEmpty();
	int Number = GameMode->QuestManager->GetStartedQuests().Num();
	for (int i = 0; i < Number; i++)
	{
		Slots.Add(CreateWidget(GetWorld(), BP_Slot));
		ScrollBox_List->AddChild(Slots.Top());
		auto QuestSlot = Cast<UWidget_PlayerQuestList>(Slots.Top());
		QuestSlot->SetParentUI(this);
		QuestSlot->SetQuestId(GameMode->QuestManager->GetStartedQuests()[i].Id);
	}

	bStartedQuest = true;
}

void UWidget_PlayerQuest::RefreshCleared()
{
	if (!bStartedQuest)
		return;

	UseGameMode
	SlotsMakeEmpty();
	int Number = GameMode->QuestManager->GetClearedQuests().Num();
	for (int i = 0; i < Number; i++)
	{
		Slots.Add(CreateWidget(GetWorld(), BP_Slot));
		ScrollBox_List->AddChild(Slots.Top());
		auto QuestSlot = Cast<UWidget_PlayerQuestList>(Slots.Top());
		QuestSlot->SetParentUI(this);
		QuestSlot->SetQuestId(GameMode->QuestManager->GetClearedQuests()[i].Id);
	}
	bStartedQuest = false;
}
void UWidget_PlayerQuest::SlotsMakeEmpty()
{
	if (Slots.IsEmpty())
		return;

	for(int i = 0; i < Slots.Num(); i++)
		Slots.Pop()->RemoveFromViewport();
}
void UWidget_PlayerQuest::CreateSlot()
{
	bStartedQuest = false;
	RefreshStarted();
}

void UWidget_PlayerQuest::SetGoalAndNow(int QuestId)
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	int Goal;

	if (QuestId == -1)
	{
		Txt_Goal->SetText(FText::FromString(TEXT("")));
		Txt_Now->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		Goal = GameMode->QuestManager->GetQuest(QuestId).TargetNum;
		Txt_Goal->SetText(FText::AsNumber(Goal));
		Txt_Now->SetText(FText::AsNumber(0));
	}
}
