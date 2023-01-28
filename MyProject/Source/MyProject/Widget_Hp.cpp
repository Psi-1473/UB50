// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Hp.h"
#include "GameFramework/Character.h"
#include "MyPlayer.h"
#include "PlayerStatComponent.h"
#include "Components/ProgressBar.h"

void UWidget_Hp::NativeConstruct()
{
	auto Character = Cast<AMyPlayer>(GetOwningLocalPlayer());
	
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	
	if (Character)
	{
		Player = Character;
		Character->Stat->OnHpDecreased.AddUObject(this, &UWidget_Hp::UpdatePlayerHp);
	}

	// ĳ���Ͱ� �÷��̾ �ƴ� ��� = ��
}

void UWidget_Hp::UpdatePlayerHp()
{

	//ProgressBar
	HpBar->SetPercent(Player->Stat->GetHpRatio());


}


