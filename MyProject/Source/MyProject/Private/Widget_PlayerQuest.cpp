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
	Btn_Started->OnClicked.AddDynamic(this, &UWidget_PlayerQuest::RefreshStarted);
	Btn_Completed->OnClicked.AddDynamic(this, &UWidget_PlayerQuest::RefreshCleared);

	CreateSlot();
}

void UWidget_PlayerQuest::ChangeInfo(int QuestId)
{
	if (ViewQuestId == QuestId)
		return;
	ViewQuestId = QuestId;
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	FString Type = GameMode->QuestManager->GetQuest(QuestId)->TypeName;
	FString Sub = GameMode->QuestManager->GetQuest(QuestId)->Sub;

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
	bStartedQuest = true;

	UseGameMode
	SlotsMakeEmpty();
	int Number = GameMode->QuestManager->NumOfStarted();//여기서 에러 터졌음

	for (int i = 0; i < Number; i++)
	{
		Slots.Add(CreateWidget(GetWorld(), BP_Slot));
		ScrollBox_List->AddChild(Slots.Top());
		auto QuestSlot = Cast<UWidget_PlayerQuestList>(Slots.Top());
		QuestSlot->SetParentUI(this);
		// 여기서 아이디 값을 이상하게 가져오나?
		// ㅇㅇ 왜??

		UE_LOG(LogTemp, Warning, TEXT("StartedNumber : %d"), GameMode->QuestManager->StartedQuests[i].Id);
		//QuestSlot->SetQuestId(GameMode, GameMode->QuestManager->GetStartedQuest(i)->Id);
	}

	//TArray<Quest>* QuestList = GameMode->QuestManager->GetStartedQuests();
	//
	//if (QuestList->IsEmpty())
	//	return;
	//
	//for(Quest MyQuest : *QuestList)
	//{
	//	Slots.Add(CreateWidget(GetWorld(), BP_Slot));
	//	ScrollBox_List->AddChild(Slots.Top());
	//	auto QuestSlot = Cast<UWidget_PlayerQuestList>(Slots.Top());
	//	QuestSlot->SetParentUI(this);
	//
	//	UE_LOG(LogTemp, Warning, TEXT("StartedNumber : %d"), MyQuest.Id);
	//	UE_LOG(LogTemp, Warning, TEXT("StartedNumber : %d"), MyQuest.Id);
	//}


}

void UWidget_PlayerQuest::RefreshCleared()
{
	if (!bStartedQuest)
		return;
	bStartedQuest = false;

	UseGameMode
	SlotsMakeEmpty();
	int Number = GameMode->QuestManager->NumOfCleared();
	for (int i = 0; i < Number; i++)
	{
		Slots.Add(CreateWidget(GetWorld(), BP_Slot));
		ScrollBox_List->AddChild(Slots.Top());
		auto QuestSlot = Cast<UWidget_PlayerQuestList>(Slots.Top());
		QuestSlot->SetParentUI(this);
		FQuest* q = GameMode->QuestManager->GetClearedQuest(i);
		QuestSlot->SetQuestId(GameMode, q->Id);
	}
}
void UWidget_PlayerQuest::SlotsMakeEmpty()
{
	if (Slots.IsEmpty())
		return;

	while(!Slots.IsEmpty())
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
	int Now;

	if (QuestId == -1)
	{
		Txt_Goal->SetText(FText::FromString(TEXT("")));
		Txt_Now->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		Goal = GameMode->QuestManager->GetQuest(QuestId)->TargetNum;
		Now = GameMode->QuestManager->GetStartedQuestById(QuestId).NowNum;
		Txt_Goal->SetText(FText::AsNumber(Goal));
		Txt_Now->SetText(FText::AsNumber(Now));
	}
}
