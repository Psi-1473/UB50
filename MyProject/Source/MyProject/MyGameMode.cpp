// Fill out your copyright notice in the Description page of Project Settings.



#include "MyGameMode.h"
#include "Widget_PlayerMain.h"
#include "Widget_Inventory.h"

AMyGameMode::AMyGameMode()
{
	//DefaultPawnClass = BP_MyPlayer::StaticClass();
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_Inventory.WBP_Inventory_C'"));
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
			UWidget_Inventory* Inven = Cast<UWidget_Inventory>(CurrentWidget);
			Inven->CreateSlot();
			// Add to Viewport ¹Ý´ë = RemoveFromViewport

		}
	}
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	

}

void AMyGameMode::UIUpdate_Hp(float Value)
{
	//UWidget_PlayerMain* Hud = Cast<UWidget_PlayerMain>(CurrentWidget);
	//
	//Hud->UpdateHp(Value);
}
