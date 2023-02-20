// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Shop.h"
#include "Components/ScrollBox.h"

void UWidget_Shop::NativeConstruct()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SA(TEXT("WidgetBlueprint'/Game/UI/ShopSlot.ShopSlot_C'"));

	if (SA.Succeeded())
	{
		BP_Slot = SA.Class;
	}
}

void UWidget_Shop::CreateSlot()
{
	Slot = CreateWidget(GetWorld(), BP_Slot);
	
	Scroll_List->AddChild(Slot);
	UE_LOG(LogTemp, Warning, TEXT("Shop Slot"));
}
