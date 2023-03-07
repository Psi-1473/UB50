// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


public:
	int8 GetInvenType() { return TypeIndex; }
	void CreateSlot();
	void AddItem();//플레이어에 아이템 정보 저장하는 리스트 생성후 그 리스트랑 비교 후 채우기
	void RemoveItem(int8 Index);
	void RefreshSlot(int8 ItemType); // 아이템 리스트랑 비교해서 아이템 들어있으면 아이템 없으면 null로
	
public:
	UFUNCTION()
	void RefreshToWeapon();

	UFUNCTION()
	void RefreshToArmor();

	UFUNCTION()
	void RefreshToUse();

	void ChangeGold(int Value);
	void ChangeWeapon(int Id, class UMyGameInstance* GInstance);
	void ChangeArmor(int Id, class UMyGameInstance* GInstance);


private:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* WrapBox_Slot;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Weapon;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Armor;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Use; // 잠시 방어구로 사용

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Gold;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_EquipWeapon;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_EquipArmor;

	UPROPERTY()
	int8 TypeIndex = 0; // 0 - 무기, 1 - 장비, 2 - 소모품

	int MaxInvenSlot = 24;
	int Gold;
	
public:
	UPROPERTY()
	TArray<class UWidget_InvenSlot*> Slots;
	//이거 안해도 될 듯
	// 아이템 삭제시 -> 클릭한 슬롯의 데이터 삭제하고 그 번호와 아이템 타입에 해당하는 정보를 플레이어에서 삭제
	// ex) 인벤토리 장비탭 4번째칸 아이템을 버린다
	// -> 해당 슬롯 정보 초기화 -> 인벤토리에서 빈 것처럼 보이게 된다.
	// -> 플레이어에 접근해서 장비아이템 리스트의 4번째 아이템 null로 -> 실제로 빈 것이 된다.


};
