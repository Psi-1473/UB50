// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Quest.h"
#include "../MyPlayer.h"
#include "../MyGameMode.h"
#include "../MyGameInstance.h"
#include "Components/ScrollBox.h"
#include "Widget_QuestSlot.h"
#include "Kismet/GameplayStatics.h"

UWidget_Quest::UWidget_Quest(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> QUESTLIST(TEXT("WidgetBlueprint'/Game/UI/QuestList.QuestList_C'"));
	if (QUESTLIST.Succeeded())
	{
		BP_Slot = QUESTLIST.Class;
	}
}

void UWidget_Quest::NativeConstruct()
{
	SetNpcId();
	CreateQuestList();
	UE_LOG(LogTemp, Warning, TEXT("Open Quest UI : Native Constructed"));
}

void UWidget_Quest::SetNpcId()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Quest UI : Interacting"));
}

void UWidget_Quest::CreateQuestList()
{
	Slots.Empty();
	UseGameMode
	UseGInstance
	if (GInstance->QuestManager->GetQuestsByNpcId(NpcId).IsEmpty())
		return;
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	TArray<int> QuestIds = MyPlayer->GetInteractNpc()->GetPossibleQuests();
	TArray<int> CanQuestIds = MyPlayer->GetInteractNpc()->GetCanClearQuests();
	for (int i = 0; i < CanQuestIds.Num(); i++)
	{
		Slots.Add(CreateWidget(GetWorld(), BP_Slot));
		ScrollBox_ListBack->AddChild(Slots.Top());
		auto QuestSlot = Cast<UWidget_QuestSlot>(Slots.Top());
		QuestSlot->SetClear();
		QuestSlot->SetId(CanQuestIds[i]);
		UE_LOG(LogTemp, Warning, TEXT(" Set Clear Quest! "));
	}
	for (int i = 0; i < QuestIds.Num(); i++)
	{
		Slots.Add(CreateWidget(GetWorld(), BP_Slot));
		ScrollBox_ListBack->AddChild(Slots.Top());
		auto QuestSlot = Cast<UWidget_QuestSlot>(Slots.Top());
		QuestSlot->SetId(QuestIds[i]);
	}
}

void UWidget_Quest::Refresh()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots.Pop()->RemoveFromViewport();
	}

	CreateQuestList();
}
