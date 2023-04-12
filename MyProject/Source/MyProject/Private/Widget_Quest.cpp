// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Quest.h"
#include "../MyPlayer.h"
#include "../MyGameMode.h"
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
	;
	UE_LOG(LogTemp, Warning, TEXT("Open Quest UI : Interacting"));
}

void UWidget_Quest::CreateQuestList()
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode->QuestManager->GetQuestsByNpcId(NpcId).IsEmpty())
		return;
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	TArray<int> QuestIds = MyPlayer->GetInteractNpc()->GetPossibleQuests();

	for (int i = 0; i < QuestIds.Num(); i++)
	{
		Slot = CreateWidget(GetWorld(), BP_Slot);
		ScrollBox_ListBack->AddChild(Slot);
		auto QuestSlot = Cast<UWidget_QuestSlot>(Slot);
		QuestSlot->SetId(QuestIds[i]);
	}
}
