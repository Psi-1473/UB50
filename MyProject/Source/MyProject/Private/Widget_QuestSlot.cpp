// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_QuestSlot.h"
#include "Kismet/GameplayStatics.h"
#include "../MyGameMode.h"
#include "../MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Widget_YesOrNo.h"
#include "../Define.h"
#include "Manager_Quest.h"

void UWidget_QuestSlot::NativeConstruct()
{
	//Init();
	Btn_Quest->OnClicked.AddDynamic(this, &UWidget_QuestSlot::StartScript);
}

void UWidget_QuestSlot::SetId(int Id)
{
	QuestId = Id;
	Init();
}

void UWidget_QuestSlot::Init()
{
	UseGInstance
	FQuest* MyQuest = GInstance->QuestManager->GetQuest(QuestId);
	Txt_Name->SetText(FText::FromString(MyQuest->Name));
	Txt_Sub->SetText(FText::FromString(MyQuest->Sub));
	if (!bCleared)
		Img_Clear->SetVisibility(ESlateVisibility::Hidden);
}

void UWidget_QuestSlot::StartScript()
{
	if (bCleared)
	{
		PopupYesOrNo();
		return;
	}

	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	MyPlayer->CloseUI(UIType::QUEST);
	MyPlayer->SetState(SCRIPT);
	int NpcId = MyPlayer->GetInteractNpc()->GetNpcId();
	//Conv¿¡¼­ Buttonµé ²ô±â
	UE_LOG(LogTemp, Warning, TEXT("Äù½ºÆ® ID : %d Å¬¸¯"), QuestId);
	UseGInstance
	GInstance->ScriptManager->StartScript(GInstance, MyPlayer, NpcId, QuestId);
}

void UWidget_QuestSlot::PopupYesOrNo()
{
	UE_LOG(LogTemp, Warning, TEXT("Try YES OR NO"));
	UseGInstance
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UUserWidget* YesNoWidget = GInstance->UIManager->PopupUI(MyPlayer->GetWorld(), UIType::YESNO);

	if (YesNoWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success!!!!!! YES OR NO"));
	}

	auto YNWidget = Cast<UWidget_YesOrNo>(YesNoWidget);

	YNWidget->SetValue(YesOrNoType::CLEAR);
	YNWidget->Init();
	YNWidget->SetQuestId(QuestId);

}

