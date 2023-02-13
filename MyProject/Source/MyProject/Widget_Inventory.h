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
	void AddItem();//�÷��̾ ������ ���� �����ϴ� ����Ʈ ������ �� ����Ʈ�� �� �� ä���
	void RemoveItem(int8 Index);
	void RefreshSlot(int8 ItemType); // ������ ����Ʈ�� ���ؼ� ������ ��������� ������ ������ null��
	
private:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* WrapBox_Slot;

	UPROPERTY()
	int8 TypeIndex = 0; // 0 - ����, 1 - ���, 2 - �Ҹ�ǰ
	
public:
	UPROPERTY()
	TArray<class UWidget_InvenSlot*> Slots;
	//�̰� ���ص� �� ��
	// ������ ������ -> Ŭ���� ������ ������ �����ϰ� �� ��ȣ�� ������ Ÿ�Կ� �ش��ϴ� ������ �÷��̾�� ����
	// ex) �κ��丮 ����� 4��°ĭ �������� ������
	// -> �ش� ���� ���� �ʱ�ȭ -> �κ��丮���� �� ��ó�� ���̰� �ȴ�.
	// -> �÷��̾ �����ؼ� �������� ����Ʈ�� 4��° ������ null�� -> ������ �� ���� �ȴ�.


};
