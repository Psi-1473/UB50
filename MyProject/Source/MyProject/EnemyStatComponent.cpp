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
	Super::BeginPlay();

	// ...
}

void UEnemyStatComponent::InitializeComponent()
{
	Level = 1;
	SetLevel(Level);
}

void UEnemyStatComponent::SetLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(Level);
		if (StatData)
		{
			Level = StatData->Level;
			Hp = StatData->MaxHp;
			Attack = StatData->Attack;
		}
	}
}

void UEnemyStatComponent::OnAttacked(float DamagedAmount)
{
	Hp -= DamagedAmount;
	if (Hp < 0)
		Hp = 0;

	UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), Hp);
}

