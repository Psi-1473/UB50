// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayerMain.h"
#include "Components/ProgressBar.h"


void UWidget_PlayerMain::UpdateHp(float Percentage)
{
	PB_Hp->SetPercent(Percentage);
}
