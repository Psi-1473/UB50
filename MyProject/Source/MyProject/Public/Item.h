// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class MYPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	int GetNumber() { return Number; }
	int GetInvenIndex() { return InvenIndex; }
	bool IsZero() { return Number <= 0; }

	void SetInfo(struct FUseItemData* ItemData); // 수정 예정
	void UpNumber(int Value = 1);
	void DownNumber(int Value = 1);
	void SetInvenIndex(int Value) { InvenIndex = Value; }

public:
	int GetId() { return Id; }

private:
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
		int Id;

	UPROPERTY(EditAnywhere)
		int Hp;

	UPROPERTY(EditAnywhere)
		int Mp;

	UPROPERTY(EditAnywhere)
		int SellPrice;

	UPROPERTY(EditAnywhere)
		int BuyPrice;

	UPROPERTY(EditAnywhere)
		int Number;

	UPROPERTY(EditAnywhere)
		int InvenIndex;
};
