// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayerQuest.h"
#include "../MyGameMode.h"
#include "Manager_Quest.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Widget_PlayerQuestList.h"
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

void UWidget_PlayerQuest::CreateSlot()
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	int Number = GameMode->QuestManager->GetStartedQuest().Num();

	for (int i = 0; i < Number; i++)
	{
		Slot = CreateWidget(GetWorld(), BP_Slot);
		ScrollBox_List->AddChild(Slot);
		auto QuestSlot = Cast<UWidget_PlayerQuestList>(Slot);
		QuestSlot->SetParentUI(this);
		QuestSlot->SetQuestId(GameMode->QuestManager->GetStartedQuest()[i]);
	}
}

void UWidget_PlayerQuest::SetGoalAndNow(int QuestId)
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	int Goal;

	if (QuestId == -1)
		Goal = 0;
	else
		Goal = GameMode->QuestManager->GetQuest(QuestId).TargetNum;

	Txt_Goal->SetText(FText::AsNumber(Goal));
	Txt_Now->SetText(FText::AsNumber(0));

}
