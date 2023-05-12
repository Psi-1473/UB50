// Fill out your copyright notice in the Description page of Project Settings.



#include "MyGameMode.h"
#include "Widget_PlayerMain.h"
#include "Widget_Inventory.h"
#include "Manager_Inven.h"
#include "Widget_CoolSlot.h"

AMyGameMode::AMyGameMode()
{
	//DefaultPawnClass = BP_MyPlayer::StaticClass();

	
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_PlayerMain.WBP_PlayerMain_C'"));
	UE_LOG(LogTemp, Warning, TEXT("GameMode Init!"));
	if (UW.Succeeded())
	{
		HUD_Class = UW.Class;

		CurrentWidget = CreateWidget(GetWorld(), HUD_Class);
		if (CurrentWidget)
			CurrentWidget->AddToViewport();
	}
}

AMyGameMode::~AMyGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode Destroyed!"));
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	UseGInstance
		GInstance->UIManager->Init();
	GInstance->QuestManager->ClearNpc();
	UE_LOG(LogTemp, Warning, TEXT("GameMode Begin!"));
	UWidget_PlayerMain* Hud = Cast<UWidget_PlayerMain>(CurrentWidget);

	Hud->SetQuickId();

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
	UWidget_PlayerMain* Hud = Cast<UWidget_PlayerMain>(CurrentWidget);

	Hud->SetImageAlpha(2, true);
	Hud->ChangeEText(Value);
}

void AMyGameMode::UsePotion(int KeyNumber)
{
	UWidget_PlayerMain* Hud = Cast<UWidget_PlayerMain>(CurrentWidget);

	Hud->ClickQuickSlot(KeyNumber);
}

void AMyGameMode::ChangeQuickIndex(int KeyNumber, int ChangedIndex)
{
	UWidget_PlayerMain* Hud = Cast<UWidget_PlayerMain>(CurrentWidget);

	Hud->ChangeQuickIndex(KeyNumber, ChangedIndex);
}

