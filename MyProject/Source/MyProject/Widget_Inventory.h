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
	void AddItem();//�÷��̾ ������ ���� �����ϴ� ����Ʈ ������ �� ����Ʈ�� �� �� ä���
	void RemoveItem(int8 Index);
	void RefreshSlot(int8 ItemType); // ������ ����Ʈ�� ���ؼ� ������ ��������� ������ ������ null��
	
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
	class UButton* Btn_Use; // ��� ���� ���

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Gold;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_EquipWeapon;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_EquipArmor;

	UPROPERTY()
	int8 TypeIndex = 0; // 0 - ����, 1 - ���, 2 - �Ҹ�ǰ

	int MaxInvenSlot = 24;
	int Gold;
	
public:
	UPROPERTY()
	TArray<class UWidget_InvenSlot*> Slots;
	//�̰� ���ص� �� ��
	// ������ ������ -> Ŭ���� ������ ������ �����ϰ� �� ��ȣ�� ������ Ÿ�Կ� �ش��ϴ� ������ �÷��̾�� ����
	// ex) �κ��丮 ����� 4��°ĭ �������� ������
	// -> �ش� ���� ���� �ʱ�ȭ -> �κ��丮���� �� ��ó�� ���̰� �ȴ�.
	// -> �÷��̾ �����ؼ� �������� ����Ʈ�� 4��° ������ null�� -> ������ �� ���� �ȴ�.


};
