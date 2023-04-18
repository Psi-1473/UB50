// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Quest.h"
#include "../MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Dom/JsonValue.h"


UManager_Quest::UManager_Quest()
{

	//LoadQuestData();
}

void UManager_Quest::LoadQuestData()
{
	FString FileStr;
	FString FilePath;
	FilePath = TEXT("C:/Users/Percy/Desktop/UnrealProjects/UB50/MyProject/Content/Data/Json/Quest/Quest.json");
	FFileHelper::LoadFileToString(FileStr, *FilePath);

	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(FileStr);
	TSharedPtr<FJsonObject> jsonObj;

	jsonObj = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(jsonReader, jsonObj) && jsonObj.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Json Deserialized"));

		TArray<TSharedPtr<FJsonValue>> jsonQuestItems = jsonObj->GetArrayField(TEXT("quests"));
		for (int i = 0; i < jsonQuestItems.Num(); i++)
		{
			TSharedPtr<FJsonObject> jsonItem = jsonQuestItems[i]->AsObject();
			FQuest q;
			jsonItem->TryGetNumberField(TEXT("Id"), q.Id);
			jsonItem->TryGetNumberField(TEXT("NpcId"), q.NpcId);
			jsonItem->TryGetNumberField(TEXT("CompleteNpcId"), q.CompleteNpcId);
			jsonItem->TryGetStringField(TEXT("Type"), q.TypeName);
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
			q.CanClear = false;


			Quests.Add(q.Id, q);


			if (!QuestsByNpcId.Contains(q.NpcId))
			{
				FNpcQuest Nq;
				QuestsByNpcId.Add(q.NpcId, Nq);
			}
			QuestsByNpcId[q.NpcId].NpcQuestList.Add(q);
		}

		UE_LOG(LogTemp, Log, TEXT("Quest Files Saved %d %d"), Quests[0].Gold, Quests[1].Gold);
		UE_LOG(LogTemp, Log, TEXT("Quest Files Saved %d"), QuestsByNpcId[0].NpcQuestList.Num());
	}


}

void UManager_Quest::SpreadToNpc()
{
}

void UManager_Quest::PlayerTakesQuest(int QuestId)
{
	FQuest NewQuest = Quests[QuestId];

	StartedQuests.Add(NewQuest);
	int NpcId = NewQuest.NpcId;
	GetNpcById(NpcId)->RemovePossibleQuest(QuestId);

	if (NewQuest.TypeName == "Normal")
		StartedToClear(NewQuest.Id);
}

void UManager_Quest::StartedToClear(int QuestId)
{
	int Idx = FindQuestIndexById(StartedQuests, QuestId);
	if (StartedQuests[Idx].CanClear)
		return;
	StartedQuests[Idx].CanClear = true;

	GetNpcById(StartedQuests[Idx].CompleteNpcId)->AddToCanClearQuest(QuestId);
}

void UManager_Quest::ClearQuest(int QuestId)
{
	int Idx = FindQuestIndexById(StartedQuests, QuestId);
	if (Idx == -1)
		return;
	int NpcId = Quests[QuestId].CompleteNpcId;

	ClearedQuests.Add(Quests[QuestId]);
	StartedQuests.RemoveAt(Idx);
	GetNpcById(NpcId)->RemoveCanClearQuest(QuestId);
	UnlockNextQuest(QuestId);
	UE_LOG(LogTemp, Warning, TEXT(" StartedQuest Num : %d"), StartedQuests.Num());
}

void UManager_Quest::UnlockNextQuest(int QuestId)
{
	int NextQuestId = Quests[QuestId].Next;
	if (NextQuestId == -1)
		return;

	Quests[NextQuestId].Locked = false;
	int NpcId = Quests[NextQuestId].NpcId;
	GetNpcById(NpcId)->AddToPossibleQuest(NextQuestId);
}

void UManager_Quest::AddQuestTargetNum(FString QType, int TargetId)
{
	for (int i = 0; i < StartedQuests.Num(); i++)
	{
		if (StartedQuests[i].TypeName != QType)
			continue;

		if (StartedQuests[i].TargetId != TargetId)
			continue;

		StartedQuests[i].NowNum++;
		UE_LOG(LogTemp, Warning, TEXT(" TargetNum Up "));

		if (StartedQuests[i].NowNum >= StartedQuests[i].TargetNum)
			StartedToClear(StartedQuests[i].Id);
	}
}

FQuest UManager_Quest::GetStartedQuestById(int QuestId)
{
	int Idx = FindQuestIndexById(StartedQuests, QuestId);
	if (Idx == -1)
	{
		FQuest q;
		q.Id = -1;
		return q;
	}

	return StartedQuests[Idx];
}

TArray<FQuest> UManager_Quest::GetQuestsByNpcId(int NpcId)
{
	if (QuestsByNpcId.Contains(NpcId))
		return QuestsByNpcId[NpcId].NpcQuestList;
	return TArray<FQuest>();
}

int UManager_Quest::FindQuestIndexById(TArray<FQuest> Arr, int QuestId)
{
	for (int i = 0; i < Arr.Num(); i++)
	{
		if (Arr[i].Id == QuestId)
			return i;
	}
	return -1;
}