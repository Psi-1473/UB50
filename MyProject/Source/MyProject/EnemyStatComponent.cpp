// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatComponent.h"

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

}

void UEnemyStatComponent::SetLevel(int32 NewLevel)
{
}

void UEnemyStatComponent::OnAttacked(float DamagedAmount)
{
}

