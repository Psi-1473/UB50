// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct FNpcScriptData
{
public:
		int NpcId;
		int QuestId;
		FString Line;
};



class MYPROJECT_API Manager_Script
{
public:
	Manager_Script();
	~Manager_Script();

public:
	void LoadScriptData(class UMyGameInstance* GInstance, int NpcId);
	void StartScript(class UMyGameInstance* GInstance, class AMyPlayer* Player, int NpcId, int QuestId);
	void NextScript(class UMyGameInstance* GInstance, class AMyPlayer* Player);

	void SetScriptEmpty();


private:
	int ScriptingQuestId;

	TArray<FNpcScriptData> NowScripts;
	int GoalPage;
	int Page = 0;
};
