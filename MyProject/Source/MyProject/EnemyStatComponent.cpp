// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyStatComponent::UEnemyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UEnemyStatComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	// ...
}

void UEnemyStatComponent::InitializeComponent()
{

}

void UEnemyStatComponent::SetMonster(FString MobName)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	// TODO 얘가 컴포넌트니까 오너 폰 받아와서 걔를 통해 데이터 불러오기 
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetEnemyData(MobName);
		if (StatData)
		{
			Id = StatData->Id;
			Name = StatData->Name;
			Level = StatData->Level;
			MaxHp = StatData->MaxHp;
			SetHp(MaxHp);
			Attack = StatData->Attack;
			MaxGold = StatData->MaxGold;
			MinGold = StatData->MinGold;
		}
	}
}

void UEnemyStatComponent::SetHp(float NewHp)
{
	Hp = NewHp;
	if (Hp < 0)
		Hp = 0;


	OnHpDecreased.Broadcast();
}

void UEnemyStatComponent::OnAttacked(float DamagedAmount)
{
	SetHp(Hp - DamagedAmount);
	if (Hp < 0)
		Hp = 0;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Attacked %d"), Hp);
}



