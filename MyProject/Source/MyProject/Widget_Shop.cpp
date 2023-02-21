// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Shop.h"
#include "Components/ScrollBox.h"

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

void UWidget_Shop::CreateSlot(int Num)
{
	for (int i = 0; i < Num; i++)
	{
		Slot = CreateWidget(GetWorld(), BP_Slot);

		Scroll_List->AddChild(Slot);
	}
	UE_LOG(LogTemp, Warning, TEXT("Shop Slot"));
}
