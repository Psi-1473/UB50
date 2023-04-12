// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_QuestSlot.h"
#include "Kismet/GameplayStatics.h"
#include "../MyGameMode.h"
#include "Components/TextBlock.h"
#include "Manager_Quest.h"

void UWidget_QuestSlot::NativeConstruct()
{
	Init();
}

void UWidget_QuestSlot::Init()
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Quest MyQuest = GameMode->QuestManager->GetQuest(QuestId);
	Txt_Name->SetText(FText::FromString(MyQuest.Name));
	Txt_Sub->SetText(FText::FromString(MyQuest.Sub));
}

