// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Shop.h"
#include "Components/ScrollBox.h"
#include "Widget_ShopSlot.h"
#include "Npc.h"
#include "MyGameInstance.h"
#include "MyPlayer.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UWidget_Shop::UWidget_Shop(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SA(TEXT("WidgetBlueprint'/Game/UI/ShopSlot.ShopSlot_C'"));
	if (SA.Succeeded())
	{
		BP_Slot = SA.Class;
	}
}

void UWidget_Shop::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UWidget_Shop::NativeConstruct()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	CreateSlot(MyPlayer->GetInteractNpc());

	Btn_Exit->OnClicked.AddDynamic(this, &UWidget_Shop::CloseUI);
}

void UWidget_Shop::NativeDestruct()
{
	Scroll_List->ClearChildren();
}

void UWidget_Shop::CreateSlot(ANpc* NPC)
{
	if (NPC == nullptr)
		return;
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());

	int ItemType = NPC->ItemType;
	int SlotSize = NPC->GetTotalItemNum();

	for (int i = 0; i < SlotSize; i++)
	{
		Slot = CreateWidget(GetWorld(), BP_Slot);
		Scroll_List->AddChild(Slot);
		auto ShopSlot = Cast<UWidget_ShopSlot>(Slot);
		ShopSlot->InitializeSlot(GInstance, ItemType, NPC->GetItem(i));
	}
	UE_LOG(LogTemp, Warning, TEXT("Shop Slot"));
}

void UWidget_Shop::CloseUI()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	MyPlayer->CloseUI(SHOP);
}
