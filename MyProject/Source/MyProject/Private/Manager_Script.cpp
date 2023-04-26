// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Script.h"
#include "Manager_Quest.h"
#include "../MyGameInstance.h"
#include "../DEFINE.H"

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
