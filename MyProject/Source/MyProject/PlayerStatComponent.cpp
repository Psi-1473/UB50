// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;

	Level = 1;
	// ...
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerStatComponent::InitializeComponent()
{
	Level = 1;
	SetLevel(Level);
}

void UPlayerStatComponent::SetLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(Level);
		if (StatData)
		{
			Level = StatData->Level;
			MaxHp = StatData->MaxHp;
			SetHp(StatData->MaxHp - 50);
			Attack = StatData->Attack;
		}
	}
}

void UPlayerStatComponent::SetHp(int32 NewHp)
{
	Hp = NewHp;
	if (Hp < 0)
		Hp = 0;

	if (Hp > MaxHp)
		Hp = MaxHp;

	OnHpDecreased.Broadcast();
}

void UPlayerStatComponent::OnAttacked(float DamagedAmount)
{
	SetHp(Hp - DamagedAmount);
	if (Hp < 0)
		Hp = 0;

	UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), Hp);
}


