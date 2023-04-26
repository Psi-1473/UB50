// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Script.h"
#include "Manager_Quest.h"
#include "Manager_UI.h"
#include "Kismet/GameplayStatics.h"
#include "../MyPlayer.h"
#include "../MyGameInstance.h"
#include "../DEFINE.H"
#include "Widget_YesOrNo.h"

Manager_Script::Manager_Script()
{
}

Manager_Script::~Manager_Script()
{
}

void Manager_Script::LoadScriptData(UMyGameInstance* GInstance, int NpcId)
{
	FString FileName = FString::FromInt(NpcId) + TEXT(".json");
	FString FileStr;
	FString FilePath;
	FilePath = TEXT("C:/Users/Percy/Desktop/UnrealProjects/UB50/MyProject/Content/Data/Json/NpcScripts/") + FileName;
	FFileHelper::LoadFileToString(FileStr, *FilePath);
	UE_LOG(LogTemp, Warning, TEXT("Path : %s"), *FilePath);
	if (FileStr.IsEmpty())
		return;

	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(FileStr);
	TSharedPtr<FJsonObject> jsonObj;

	jsonObj = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(jsonReader, jsonObj) && jsonObj.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Json Deserialized"));

		TArray<TSharedPtr<FJsonValue>> jsonQuestItems = jsonObj->GetArrayField(TEXT("scripts"));
		
		for (int i = 0; i < jsonQuestItems.Num(); i++)
		{
			TSharedPtr<FJsonObject> jsonItem = jsonQuestItems[i]->AsObject();
			FNpcScriptData s;
			jsonItem->TryGetNumberField(TEXT("NpcId"), s.NpcId);
			jsonItem->TryGetNumberField(TEXT("QuestId"), s.QuestId);
			jsonItem->TryGetNumberField(TEXT("ThisPage"), s.ThisPage);
			jsonItem->TryGetNumberField(TEXT("GoalPage"), s.GoalPage);
			jsonItem->TryGetStringField(TEXT("Line"), s.Line);

			GInstance->QuestManager->GetNpcById(NpcId)->AddScriptData(s);
		}

		UE_LOG(LogTemp, Warning, TEXT("Npc Script Load Successed!"));
	}
}

void Manager_Script::StartScript(UMyGameInstance* GInstance, AMyPlayer* Player, int NpcId, int QuestId)
{
	GInstance->UIManager->OffConvButton();
	ANpc* Npc = GInstance->QuestManager->GetNpcById(NpcId);

	NowScripts = Npc->GetScriptData(QuestId);
	GoalPage = NowScripts[0].GoalPage;
	GInstance->UIManager->ChangeConvLine(NowScripts[Page].Line);
	Page++;
}

void Manager_Script::NextScript(UMyGameInstance* GInstance, AMyPlayer* Player)
{
	if (Page >= GoalPage)
	{
		UUserWidget* YesNoWidget = GInstance->UIManager->PopupUI(Player->GetWorld(), UIType::YESNO);
		if (YesNoWidget == nullptr) return;

		auto YNWidget = Cast<UWidget_YesOrNo>(YesNoWidget);
		YNWidget->SetValue(YesOrNoType::QUEST);
	}
	else
	{
		GInstance->UIManager->ChangeConvLine(NowScripts[Page].Line);
		Page++;
	}
}

void Manager_Script::SetScriptEmpty()
{
	GoalPage = 0;
	Page = 0;
	NowScripts.Empty();
}
