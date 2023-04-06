// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Conversation.h"
#include "../Npc.h"
#include "../MyPlayer.h"
#include "../MyGameMode.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"



void UWidget_Conversation::NativePreConstruct()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	if (MyPlayer)
		OwnerNpc = MyPlayer->GetInteractNpc();
}

void UWidget_Conversation::NativeConstruct()
{
	if (!OwnerNpc)
		return;

	if (OwnerNpc->GetIsShop())
	{
		InteractButton = CreateWidget(GetWorld(), BP_Button);
		VBox_Button->AddChild(InteractButton);
		// 버튼 생성
	}
	FText NameText = FText::FromString(OwnerNpc->GetName());
	Txt_Name->SetText(NameText);
}



