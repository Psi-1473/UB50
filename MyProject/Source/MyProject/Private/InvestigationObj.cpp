// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigationObj.h"
#include "../MyPlayer.h"

// Sets default values
AInvestigationObj::AInvestigationObj()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInvestigationObj::BeginPlay()
{
	Super::BeginPlay();

}
void AInvestigationObj::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto Player = Cast<AMyPlayer>(OtherActor);

	if (Player)
		Player->SetInteractObj(this);
}

void AInvestigationObj::NotifyActorEndOverlap(AActor* OtherActor)
{
	auto Player = Cast<AMyPlayer>(OtherActor);

	if (Player)
		Player->SetInteractObj(nullptr);
}

void AInvestigationObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInvestigationObj::Interact(AMyPlayer* Player)
{
	// 
}

