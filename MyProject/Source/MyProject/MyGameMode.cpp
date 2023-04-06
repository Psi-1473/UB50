// Fill out your copyright notice in the Description page of Project Settings.



#include "MyGameMode.h"
#include "Widget_PlayerMain.h"
#include "Widget_Inventory.h"

AMyGameMode::AMyGameMode()
{
	//DefaultPawnClass = BP_MyPlayer::StaticClass();
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_PlayerMain.WBP_PlayerMain_C'"));
	UE_LOG(LogTemp, Warning, TEXT("GameMode Start!"));
	if (UW.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode Succeeded!"));
		HUD_Class = UW.Class;

		CurrentWidget = CreateWidget(GetWorld(), HUD_Class);
		if (CurrentWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode CurrentWidget Succeeded!"));
			CurrentWidget->AddToViewport();
		}
	}

	UIManager = NewObject<UManager_UI>();
	QuestManager = NewObject<UManager_Quest>();
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	QuestManager->LoadQuestData();

}

void AMyGameMode::UIUpdate_Hp(float Value)
{
	UWidget_PlayerMain* Hud = Cast<UWidget_PlayerMain>(CurrentWidget);

	Hud->UpdateHp(Value);
}

void AMyGameMode::QSkillUpdate(int Value)
{
	UWidget_PlayerMain* Hud = Cast<UWidget_PlayerMain>(CurrentWidget);

	Hud->SetImageAlpha(0, true);
	Hud->ChangeQText(Value);
	
}

void AMyGameMode::RSkillUpdate(int Value)
{
	UWidget_PlayerMain* Hud = Cast<UWidget_PlayerMain>(CurrentWidget);

	Hud->SetImageAlpha(1, true);
	Hud->ChangeRText(Value);
	
}

void AMyGameMode::ESkillUpdate(int Value)
{
	//
}

