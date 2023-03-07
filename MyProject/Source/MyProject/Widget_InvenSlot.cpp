// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_InvenSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "MyGameInstance.h"
#include "DragWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UWidget_InvenSlot::NativeConstruct()
{
	//Btn_Slot->OnClicked.AddDynamic(this, &UWidget_InvenSlot::ClickSlot);
	
}

FReply UWidget_InvenSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		auto MyPlayer = Cast<AMyPlayer>(Char);
		// 아이템 사용
		int Id = ItemId;
		int Idx = SlotIndex;

		//SetEmpty();

		switch (ItemType)
		{
		case 0:
			MyPlayer->EquipWeapon(Id, Idx);
			break;
		case 1:
			MyPlayer->EquipArmor(Id, Idx);
			break;
		case 2:
			MyPlayer->UseItem(Id, Idx);
			break;
		}
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Left Button Down"));

		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		
	}

	return Reply.NativeReply;
}

void UWidget_InvenSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging Start"));

		UDragWidget* DragOper = NewObject<UDragWidget>();
		OutOperation = DragOper;
		DragOper->DragIndex = this->SlotIndex;

		
		if (DragVisualClass != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Widget Created"));
			UWidget_InvenSlot* VisualWidget = CreateWidget<UWidget_InvenSlot>(GetWorld(), DragVisualClass);
			VisualWidget->SetVisibility(ESlateVisibility::Visible);
			
			VisualWidget->SlotIndex = this->SlotIndex;
			VisualWidget->RefreshSlot(this);
			VisualWidget->SetPositionInViewport(InMouseEvent.GetScreenSpacePosition());
			DragOper->DefaultDragVisual = VisualWidget;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging Again"));
	}
}

bool UWidget_InvenSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragWidget* DragOper = Cast<UDragWidget>(InOperation);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging End"));

	if (DragOper != nullptr)
	{
		if (DragOper->DragIndex == this->SlotIndex)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Same Return"));
			return true;
		}
			
		// 스왑
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging Success"));
		auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		auto MyPlayer = Cast<AMyPlayer>(Char);
		MyPlayer->DraggingSwap(DragOper->DragIndex, this->SlotIndex);
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging false"));
		return false;
	}
}


void UWidget_InvenSlot::SetCount(int8 Index)
{
	FString strText;
	SlotIndex = Index;
	//strText = FString::Printf(TEXT("%d"), SlotIndex);
	//Count->SetText(FText::FromString(strText));
}

void UWidget_InvenSlot::SetWeaponItem()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	ItemType = 0;

	if (MyPlayer->WeaponList[SlotIndex] != nullptr)
	{
		ChangeImage(0, MyPlayer->WeaponList[SlotIndex]->Id, GInstance, MyPlayer);
		ItemId = MyPlayer->WeaponList[SlotIndex]->Id;
	}
	else
		SetEmpty();
}

void UWidget_InvenSlot::SetArmorItem()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	ItemType = 1;

	if (MyPlayer->ArmorList[SlotIndex] != nullptr)
	{
		ChangeImage(1, MyPlayer->ArmorList[SlotIndex]->Id, GInstance, MyPlayer);
		ItemId = MyPlayer->ArmorList[SlotIndex]->Id;
	}
	else
		SetEmpty();
}

void UWidget_InvenSlot::SetUseItem()
{
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	ItemType = 2;

	if (MyPlayer->UseItemList[SlotIndex] != nullptr)
	{
		ChangeImage(2, MyPlayer->UseItemList[SlotIndex]->Id, GInstance, MyPlayer);
		ItemId = MyPlayer->UseItemList[SlotIndex]->Id;
	}
	else
		SetEmpty();
}

void UWidget_InvenSlot::SetEmpty()
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	FString strText;
	strText = FString::Printf(TEXT(""), 00);
	Count->SetText(FText::FromString(strText));
	Img_Item->SetBrush(GInstance->GetArmorImage(0)->Brush);
	ItemId = -1;
}

void UWidget_InvenSlot::RefreshSlot(UWidget_InvenSlot* FromSlot)
{
	ItemType = FromSlot->ItemType;
	ItemCount = FromSlot->ItemCount;
	ItemId = FromSlot->ItemId;
	ItemName = FromSlot->ItemName;
	ItemSub = FromSlot->ItemSub;
	Img_Item->SetBrush(FromSlot->Img_Item->Brush);

	//if (ItemType == 0)
	//	SetWeaponItem();
	//else if (ItemType == 1)
	//	SetArmorItem();
	//else if (ItemType == 2)
	//	SetUseItem();
}

void UWidget_InvenSlot::ChangeImage(int TypeIndex, int Index, UMyGameInstance* Instance, AMyPlayer* PlayerInst)
{
	FString strText;

	switch (TypeIndex)
	{
	case 0:
		strText = FString::Printf(TEXT("%d"), PlayerInst->WeaponList[SlotIndex]->Id);
		Img_Item->SetBrush(Instance->GetWeaponImage(PlayerInst->WeaponList[SlotIndex]->Id)->Brush);
		break;
	case 1:
		strText = FString::Printf(TEXT("%d"), PlayerInst->ArmorList[SlotIndex]->Id);
		Img_Item->SetBrush(Instance->GetArmorImage(PlayerInst->ArmorList[SlotIndex]->Id)->Brush);
		break;
	case 2:
		strText = FString::Printf(TEXT("%d"), PlayerInst->UseItemList[SlotIndex]->Id);
		Img_Item->SetBrush(Instance->GetUseImage(PlayerInst->UseItemList[SlotIndex]->Id)->Brush);
		break;
	default:
		break;
	}

	Count->SetText(FText::FromString(strText));
}

void UWidget_InvenSlot::ClickSlot()
{
	UE_LOG(LogTemp, Warning, TEXT("Click Slot"));
}
