// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
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
	MOVING,
	ATTACK,
	SKILL,
	DAMAGED,
};

const enum UIType
{
	INVENTORY = 1,
	CONVERSATION = 2,
	SHOP = 3,
};
const enum BossCoolTime
{
	ALLSKILL = 10,
	SKILL1 = 20,
	SKILL2 = 30,
	SKILL3 = 30,
	SKILL4 = 40,
};