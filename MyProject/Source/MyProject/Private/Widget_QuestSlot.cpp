// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_QuestSlot.h"
#include "Kismet/GameplayStatics.h"
#include "../MyGameMode.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Widget_YesOrNo.h"
#include "Manager_Quest.h"

void UWidget_QuestSlot::NativeConstruct()
{
	//Init();
	Btn_Quest->OnClicked.AddDynamic(this, &UWidget_QuestSlot::PopupYesOrNo);
}

void UWidget_QuestSlot::SetId(int Id)
{
	QuestId = Id;
	Init();
}

void UWidget_QuestSlot::Init()
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Quest MyQuest = GameMode->QuestManager->GetQuest(QuestId);
	Txt_Name->SetText(FText::FromString(MyQuest.Name));
	Txt_Sub->SetText(FText::FromString(MyQuest.Sub));
}

void UWidget_QuestSlot::PopupYesOrNo()
{
	UE_LOG(LogTemp, Warning, TEXT("Try YES OR NO"));
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UUserWidget* YesNoWidget = GameMode->UIManager->PopupUI(MyPlayer->GetWorld(), UIType::YESNO);

	if (YesNoWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success!!!!!! YES OR NO"));
	}

	auto YNWidget = Cast<UWidget_YesOrNo>(YesNoWidget);
	YNWidget->SetValue(YesOrNoType::QUEST);
	YNWidget->SetQuestId(QuestId);
}

