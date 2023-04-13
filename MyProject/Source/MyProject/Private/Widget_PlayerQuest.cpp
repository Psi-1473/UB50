// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayerQuest.h"
#include "../MyGameMode.h"
#include "Manager_Quest.h"
#include "Components/ScrollBox.h"
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
	UE_LOG(LogTemp, Warning, TEXT("ChangeInfo!!"));
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
