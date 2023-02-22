// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Shop.h"
#include "Components/ScrollBox.h"
#include "Widget_ShopSlot.h"
#include "Npc.h"
#include "MyGameInstance.h"

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

}

void UWidget_Shop::NativeDestruct()
{
	Scroll_List->ClearChildren();
}

void UWidget_Shop::CreateSlot(ANpc* NPC)
{
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
