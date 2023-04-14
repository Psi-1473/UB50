// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Npc.h"
#include "Manager_Quest.generated.h"

/**
 * 
 */

struct Quest
{
	int Id;
	int NpcId;
	int CompleteNpcId;
	FString Type;
	FString Name;
	FString Sub;
	FString ConditionSub;
	int Gold;
	int Exp;
	bool Locked;
	bool Cleared;
	int TargetId;
	int TargetNum;
	int NowNum;
	int Next;
	bool CanClear = false;
};

UCLASS()
class MYPROJECT_API UManager_Quest : public UObject
{
	GENERATED_BODY()

public:
	UManager_Quest();

public:
	void LoadQuestData();
	void SpreadToNpc();

	void PlayerTakesQuest(int QuestId);
	void StartedToClear(int QuestId);
	void ClearQuest(int QuestId);
	void UnlockNextQuest(int QuestId);

	void AddQuestTargetNum(FString QType, int TargetId);


public:
	void AddNpc(int Key, ANpc* Npc) { NpcsInField.Add(Key, Npc); }
	class ANpc* GetNpcById(int Key) { return NpcsInField[Key]; }

	Quest GetQuest(int id) { return Quests[id]; }
	TArray<Quest> GetQuestsByNpcId(int NpcId);

	TArray<Quest> GetStartedQuests() { return StartedQuests; }
	Quest GetStartedQuestById(int QuestId);
	TArray<Quest> GetClearedQuests() { return ClearedQuests; }

	int FindQuestIndexById(TArray<Quest> Arr, int QuestId);


private:
	TMap<int, ANpc*> NpcsInField;

private:
	FString FileStr;
	FString FilePath;
	TSharedPtr<FJsonObject> jsonObj;

	TMap<int, Quest> Quests;
	TMap<int, TArray<Quest>> QuestsByNpcId;

	TArray<Quest> StartedQuests;
	TArray<Quest> ClearedQuests;
};
