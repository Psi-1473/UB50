// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInstance.h"

#define UseGameMode AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
#define UseGInstance UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
/**
 * 
 */
const int WIDGETCOUNT = 10;

const int ID_NONE = -1;

const int PLAYERSKILL_R = 0;
const int PLAYERSKILL_Q = 1;
const int PLAYERSKILL_E = 2;

const int INVENTORYNUMBER = 24;

const enum PlayerItemType
{
	WEAPON = 0,
	ARMOR = 1,
	USEITEM = 2,
};

const enum STATE
{
	IDLE,
	PATROL,
	MOVING,
	LOOT,
	SCRIPT,
	ATTACKREADY,
	ATTACK,
	SKILL,
	DAMAGED,
	STUNED,
	DIED,
};

UENUM(BlueprintType)
enum class UIType : uint8
{
	YESNO = 0 UMETA(DisplayName = "YesOrNo"),
	INVENTORY = 1 UMETA(DisplayName = "Inventory"),
	CONVERSATION = 2 UMETA(DisplayName = "Conversation"),
	SHOP = 3 UMETA(DisplayName = "Shop"),
	QUEST = 4 UMETA(DisplayName = "Quest"),
	PLAYERQUEST = 5 UMETA(DisplayName = "PlayerQuest"),
	INVESTIGATION = 6 UMETA(DisplayName = "Investigation"),

	ALL
};
const enum BossCoolTime
{
	ALLSKILL = 5,
	SKILL1 = 20,
	SKILL2 = 1,
	SKILL3 = 30,
	SKILL4 = 40,
};

const enum BossSkill
{
	SKILL_1 = 1,
	SKILL_2 = 2,
	SKILL_3 = 3,
	SKILL_4 = 4,
};

enum class YesOrNoType
{
	EXIT = 0,
	QUEST = 1,
	CLEAR = 2,
};

enum class QuestType
{
	NORMAL = 0,
	HUNTING = 1,
	COLLECT = 2,
};