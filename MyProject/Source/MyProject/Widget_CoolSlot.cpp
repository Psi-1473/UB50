// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_CoolSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture.h"
#include "DragWidget.h"
#include "DEFINE.H"
#include "Manager_Inven.h"
#include "Kismet/GameplayStatics.h"
#include "../MyPlayer.h"
#include "Widget_InvenSlot.h"


void UWidget_CoolSlot::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

bool UWidget_CoolSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragWidget* DragOper = Cast<UDragWidget>(InOperation);
	UseGInstance
	

	if (DragOper != nullptr)
	{
		if (DragOper->DragItemType == USEITEM)
		{
			SetId(DragOper->DragItemId);
			InvenIndex = DragOper->DragIndex;
			GInstance->InvenManager->QuickSlot[InvenIndex] = QuickId;
			SetImage();
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Quick Success"));

			auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			auto MyPlayer = Cast<AMyPlayer>(Char);

			MyPlayer->PlayEquipSound();
			return true;
		}
		else
			return false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging false"));
		return false;
	}
}

void UWidget_CoolSlot::SetImage()
{
	UseGInstance
	Img_Item->SetBrush(GInstance->GetUseImage(ItemId)->Brush);
}

void UWidget_CoolSlot::UseItem()
{
	if (ItemId == -1)
		return;

	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	UseGInstance
	GInstance->InvenManager->UseItem(MyPlayer, GInstance, ItemId, InvenIndex, QuickId);
}






