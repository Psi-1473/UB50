// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Investigaton.h"
#include "Components/ProgressBar.h"
#include "../DEFINE.H"
#include "Kismet/GameplayStatics.h"
#include "../MyPlayer.h"
#include "Manager_UI.h"

void UWidget_Investigaton::NativeConstruct()
{
	Super::NativeConstruct();
	GoalTime = 6.f;
	PrevTime = GetWorld()->GetTimeSeconds();
}

void UWidget_Investigaton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	NowTime = (GetWorld()->GetTimeSeconds() - PrevTime);

	UE_LOG(LogTemp, Warning, TEXT("Goal : %f, Prev : %f, Now : %f, GetTime : %f"), GoalTime, PrevTime, NowTime, GetWorld()->GetTimeSeconds());
	MyProgressBar->SetPercent(NowTime / GoalTime);

	if (NowTime >= GoalTime)
		CloseThis();
}

void UWidget_Investigaton::CloseThis()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	MyPlayer->CloseUI(UIType::INVESTIGATION);
}
