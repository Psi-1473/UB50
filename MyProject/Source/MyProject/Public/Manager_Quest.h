// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Npc.h"
#include "Manager_Quest.generated.h"

/**
 * 
 */

USTRUCT()
struct FQuest
{
	GENERATED_BODY()

	UPROPERTY()
	int Id;
	UPROPERTY()
	int NpcId;
	UPROPERTY()
	int CompleteNpcId;
	UPROPERTY()
	FString TypeName;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FString Sub;
	UPROPERTY()
	FString ConditionSub;
	UPROPERTY()
	int Gold;
	UPROPERTY()
	int Exp;
	UPROPERTY()
	bool Locked;
	UPROPERTY()
	bool Cleared;
	UPROPERTY()
	int TargetId;
	UPROPERTY()
	int TargetNum;
	UPROPERTY()
	int NowNum;
	UPROPERTY()
	int Next;
	UPROPERTY()
	bool CanClear = false;
};

USTRUCT()
struct FNpcQuest
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FQuest> NpcQuestList;
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

	FQuest* GetQuest(int id) { return &Quests[id]; }
	FString GetQuestName(int id) { return Quests[id].Name; }
	TArray<FQuest> GetQuestsByNpcId(int NpcId);

	TArray<FQuest>* GetStartedQuests() { return &StartedQuests; }

	FQuest* GetStartedQuest(int Idx) { return &StartedQuests[Idx]; }
	int NumOfStarted() { return StartedQuests.Num(); }
	inline FQuest GetStartedQuestById(int QuestId);
	FQuest* GetClearedQuest(int Idx) { return &ClearedQuests[Idx]; }
	int NumOfCleared() { return ClearedQuests.Num(); }

	int FindQuestIndexById(TArray<FQuest> Arr, int QuestId);


private:
	UPROPERTY()
	TMap<int, ANpc*> NpcsInField;

private:

	UPROPERTY()
	TMap<int, FQuest> Quests;

	UPROPERTY()
	TMap<int, FNpcQuest> QuestsByNpcId;

public:
	UPROPERTY()
	TArray<FQuest> StartedQuests;

	UPROPERTY()
	TArray<FQuest> ClearedQuests;
};
