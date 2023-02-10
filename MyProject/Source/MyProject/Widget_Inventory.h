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
	virtual void NativeOnInitialized();

public:
	void CreateSlot();
	void AddItem();//플레이어에 아이템 정보 저장하는 리스트 생성후 그 리스트랑 비교 후 채우기
	void RemoveItem(int8 Index);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* WrapBox_Slot;
	
public:
	UPROPERTY()
	TArray<class UWidget_InvenSlot*> Slots;
	//이거 안해도 될 듯
	// 아이템 삭제시 -> 클릭한 슬롯의 데이터 삭제하고 그 번호와 아이템 타입에 해당하는 정보를 플레이어에서 삭제
	// ex) 인벤토리 장비탭 4번째칸 아이템을 버린다
	// -> 해당 슬롯 정보 초기화 -> 인벤토리에서 빈 것처럼 보이게 된다.
	// -> 플레이어에 접근해서 장비아이템 리스트의 4번째 아이템 null로 -> 실제로 빈 것이 된다.


};
