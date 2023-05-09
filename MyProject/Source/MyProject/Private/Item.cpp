// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "../MyGameInstance.h"

// Sets default values
AItem::AItem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::SetInfo(FUseItemData* ItemData)
{
	Name = ItemData->Name;
	Id = ItemData->Id;
	Hp = ItemData->Hp;
	Mp = ItemData->Mp;
	SellPrice = ItemData->SellPrice;
	BuyPrice = ItemData->BuyPrice;
	Number = 0;
}

void AItem::UpNumber(int Value)
{
	Number += Value;
}

void AItem::DownNumber(int Value)
{
	Number -= Value;
	if (Number <= 0)
		Destroy();
}

