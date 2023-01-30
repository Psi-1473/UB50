// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Hp.h"
#include "GameFramework/Character.h"
#include "MyPlayer.h"
#include "Components/ProgressBar.h"
#include "EnemyCharKwang.h"


void UWidget_Hp::BindWidget_Player(UPlayerStatComponent* Stat)
{
	IsPlayer = true;
	PlayerStat = Stat;
	PlayerStat->OnHpDecreased.AddUObject(this, &UWidget_Hp::UpdateHp);
}

void UWidget_Hp::BindWidget_Enemy(UEnemyStatComponent* Stat)
{
	IsPlayer = false;
	EnemyStat = Stat;
	EnemyStat->OnHpDecreased.AddUObject(this, &UWidget_Hp::UpdateHp);
}

void UWidget_Hp::UpdateHp()
{
	if(IsPlayer)
		PB_HpBar->SetPercent(PlayerStat->GetHpRatio());
	else
		PB_HpBar->SetPercent(EnemyStat->GetHpRatio());
}



