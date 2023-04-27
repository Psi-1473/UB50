// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_YesOrNo.h"
#include "Components/Button.h"
#include "../MyGameMode.h"
#include "../MyGameInstance.h"
#include "Manager_Quest.h"
#include "Kismet/GameplayStatics.h"

void UWidget_YesOrNo::NativeConstruct()
{
	Btn_Yes->OnClicked.AddDynamic(this, &UWidget_YesOrNo::ClickYesButton);
	Btn_No->OnClicked.AddDynamic(this, &UWidget_YesOrNo::ClickNoButton);
}

void UWidget_YesOrNo::Init()
{
}

void UWidget_YesOrNo::ClickYesButton()
{
	switch (MyType)
	{
	case YesOrNoType::EXIT:
		break;
	case YesOrNoType::QUEST:
		TakeQuest();
		break;
	case YesOrNoType::CLEAR:
		ClearQuest();
		break;
	default:
		break;
	}
}

void UWidget_YesOrNo::ClickNoButton()
{
	UseGInstance
	GInstance->UIManager->CloseUI(UIType::YESNO);
}

void UWidget_YesOrNo::TakeQuest()
{
	UseGInstance
	GInstance->QuestManager->PlayerTakesQuest(QuestId);
	GInstance->UIManager->CloseUI(UIType::ALL);
	GInstance->UIManager->RefreshUI();
	UE_LOG(LogTemp, Warning, TEXT(" Take Quest! "));

	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	MyPlayer->SetState(IDLE);
	
}

void UWidget_YesOrNo::ClearQuest()
{
	UseGInstance
	GInstance->QuestManager->ClearQuest(QuestId);
	GInstance->UIManager->CloseUI(UIType::ALL);
	GInstance->UIManager->RefreshUI();
	UE_LOG(LogTemp, Warning, TEXT(" Clear Quest! "));
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	MyPlayer->SetState(IDLE);
}
