// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_InteractButton.h"
#include "../MyGameMode.h"
#include "../MyPlayer.h"
#include "../MyGameInstance.h"
#include "Manager_UI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWidget_InteractButton::NativeConstruct()
{
	Btn_Interact->OnClicked.AddDynamic(this, &UWidget_InteractButton::OpenShopUI);
}

void UWidget_InteractButton::OpenShopUI()
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UseGInstance
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	GInstance->UIManager->PopupUI(MyPlayer->GetWorld(), MyUiType);
	UE_LOG(LogTemp, Warning, TEXT("Shop Button Clicked"));
}