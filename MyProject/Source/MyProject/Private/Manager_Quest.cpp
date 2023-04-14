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
	Quest NewQuest = Quests[QuestId];

	StartedQuests.Add(NewQuest);
	int NpcId = NewQuest.NpcId;
	GetNpcById(NpcId)->RemovePossibleQuest(QuestId);

	if (NewQuest.Type == "Normal")
		StartedToClear(NewQuest.Id);


}

void UManager_Quest::StartedToClear(int QuestId)
{
	int Idx = FindQuestById(StartedQuests, QuestId);
	StartedQuests[Idx].CanClear = true;

	GetNpcById(StartedQuests[Idx].CompleteNpcId)->AddToCanClearQuest(QuestId);
	// 완료 가능 NPC의 CanClearedQuest에 추가
	UE_LOG(LogTemp, Warning, TEXT(" StartedQuest -> CanClear "));
}

void UManager_Quest::ClearQuest(int QuestId)
{
	int Idx = FindQuestById(StartedQuests, QuestId);
	if (Idx == -1)
		return;
	int NpcId = Quests[QuestId].CompleteNpcId;

	ClearedQuests.Add(Quests[QuestId]);
	StartedQuests.RemoveAt(Idx);
	GetNpcById(NpcId)->RemoveCanClearQuest(QuestId);
}

Quest UManager_Quest::GetStartedQuestById(int QuestId)
{
	int Idx = FindQuestById(StartedQuests, QuestId);
	if (Idx == -1)
	{
		Quest q;
		q.Id = -1;
		return q;
	}

	return StartedQuests[Idx];
}

int UManager_Quest::FindQuestById(TArray<Quest> Arr, int QuestId)
{
	for (int i = 0; i < Arr.Num(); i++)
	{
		if (Arr[i].Id == QuestId)
			return i;
	}
	return -1;
}

TArray<Quest> UManager_Quest::GetQuestsByNpcId(int NpcId)
{
	if (QuestsByNpcId.Contains(NpcId))
		return QuestsByNpcId[NpcId];
	return TArray<Quest>();
}
