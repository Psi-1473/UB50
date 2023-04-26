// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Conversation.h"
#include "../Npc.h"
#include "../MyPlayer.h"
#include "../MyGameMode.h"
#include "Widget_QuestSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

UWidget_Conversation::UWidget_Conversation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SHOPBTN(TEXT("WidgetBlueprint'/Game/UI/ShopButton.ShopButton_C'"));
	if (SHOPBTN.Succeeded())
		BP_Button = SHOPBTN.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> QUESTBTN(TEXT("WidgetBlueprint'/Game/UI/QuestButton.QuestButton_C'"));
	if (QUESTBTN.Succeeded())
		BP_QuestButton = QUESTBTN.Class;
}

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

	QuestButton = CreateWidget(GetWorld(), BP_QuestButton);
	VBox_Button->AddChild(QuestButton);

	FText NameText = FText::FromString(OwnerNpc->GetName());
	FText LineText = FText::FromString(OwnerNpc->GetLine());
	Txt_Name->SetText(NameText);
	Txt_Line->SetText(LineText);
}

void UWidget_Conversation::SetButtonOnOff(ESlateVisibility Value)
{
	QuestButton->SetVisibility(Value);

	if (OwnerNpc->GetIsShop())
		InteractButton->SetVisibility(Value);
}

void UWidget_Conversation::ChangeLine(FString NewLine)
{
	FText NewText = FText::FromString(NewLine);
	Txt_Line->SetText(NewText);
}



