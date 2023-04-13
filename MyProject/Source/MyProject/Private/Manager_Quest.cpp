// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Quest.h"
#include "../MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Dom/JsonValue.h"


UManager_Quest::UManager_Quest()
{
	FilePath = TEXT("C:/Users/Percy/Desktop/UnrealProjects/UB50/MyProject/Content/Data/Json/Quest/Quest.json");
}

void UManager_Quest::LoadQuestData()
{
	FFileHelper::LoadFileToString(FileStr, *FilePath);

	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(FileStr);
	jsonObj = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(jsonReader, jsonObj) && jsonObj.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Json Deserialized"));

			TArray<TSharedPtr<FJsonValue>> jsonQuestItems = jsonObj->GetArrayField(TEXT("quests"));
			for (int i = 0; i < jsonQuestItems.Num(); i++)
			{
				TSharedPtr<FJsonObject> jsonItem = jsonQuestItems[i]->AsObject();
				Quest q;
				jsonItem->TryGetNumberField(TEXT("Id"), q.Id);
				jsonItem->TryGetNumberField(TEXT("NpcId"), q.NpcId);
				jsonItem->TryGetNumberField(TEXT("CompleteNpcId"), q.CompleteNpcId);
				jsonItem->TryGetStringField(TEXT("Type"), q.Type);
				jsonItem->TryGetStringField(TEXT("Name"), q.Name);
				jsonItem->TryGetStringField(TEXT("Sub"), q.Sub);
				jsonItem->TryGetStringField(TEXT("ConditionSub"), q.ConditionSub);
				jsonItem->TryGetNumberField(TEXT("Gold"), q.Gold);
				jsonItem->TryGetNumberField(TEXT("Exp"), q.Exp);
				jsonItem->TryGetBoolField(TEXT("Locked"), q.Locked);
				jsonItem->TryGetBoolField(TEXT("Cleared"), q.Cleared);
				jsonItem->TryGetNumberField(TEXT("TargetId"), q.TargetId);
				jsonItem->TryGetNumberField(TEXT("TargetNum"), q.TargetNum);
				jsonItem->TryGetNumberField(TEXT("NowNum"), q.NowNum);
				jsonItem->TryGetNumberField(TEXT("Next"), q.Next);

				Quests.Add(q.Id, q);
				if (QuestsByNpcId.Contains(q.NpcId))
					QuestsByNpcId[q.NpcId].Add(q);
				else
				{
					TArray<Quest> qa;
					qa.Add(q);
					QuestsByNpcId.Add(q.NpcId, qa);
				}
			}

		UE_LOG(LogTemp, Log, TEXT("Quest Files Saved %d %d"), Quests[0].Gold, Quests[1].Gold);
		UE_LOG(LogTemp, Log, TEXT("Quest Files Saved %d"), QuestsByNpcId[0].Num());
	}


}

void UManager_Quest::SpreadToNpc()
{
}

void UManager_Quest::PlayerTakesQuest(int QuestId)
{
	StartedQuests.Add(QuestId);
	int NpcId = Quests[QuestId].NpcId;
	UE_LOG(LogTemp, Log, TEXT("ACCESS NPC ID : %d"), NpcId);
	UE_LOG(LogTemp, Log, TEXT("REMOVE QUEST ID : %d"), QuestId);
	GetNpcById(NpcId)->RemovePossibleQuest(QuestId);
}

TArray<Quest> UManager_Quest::GetQuestsByNpcId(int NpcId)
{
	if (QuestsByNpcId.Contains(NpcId))
		return QuestsByNpcId[NpcId];
	return TArray<Quest>();
}
