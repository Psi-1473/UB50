// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Npc.h"

/**
 *
 */


struct FQuest
{
	int Id;

	int NpcId;

	int CompleteNpcId;

	FString TypeName;

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

struct FNpcQuest
{

public:
	TArray<FQuest> NpcQuestList;
};
class MYPROJECT_API UManager_Quest
{

public:
	UManager_Quest();

public:
	UFUNCTION()
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

	FQuest* GetQuest(int id) { return &Quests[id]; }
	FString GetQuestName(int id) { return Quests[id].Name; }
	TArray<FQuest> GetQuestsByNpcId(int NpcId);

	TArray<FQuest>* GetStartedQuests() { return &StartedQuests; }
	TArray<FQuest>* GetClearedQuests() { return &ClearedQuests; }

	FQuest* GetStartedQuest(int Idx) { return &StartedQuests[Idx]; }
	FQuest* GetClearedQuest(int Idx) { return &ClearedQuests[Idx]; }

	inline FQuest GetStartedQuestById(int QuestId);


	int FindQuestIndexById(TArray<FQuest> Arr, int QuestId);


private:
	UPROPERTY()
		TMap<int, ANpc*> NpcsInField;

private:
	TMap<int, FQuest> Quests;
	TMap<int, FNpcQuest> QuestsByNpcId;

public:
	TArray<FQuest> StartedQuests;
	TArray<FQuest> ClearedQuests;
};
