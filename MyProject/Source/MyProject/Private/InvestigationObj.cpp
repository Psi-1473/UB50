// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigationObj.h"
#include "../MyPlayer.h"
#include "../MyAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInvestigationObj::AInvestigationObj()
{
	PrimaryActorTick.bCanEverTick = true;
	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	RootComponent = TransferVolume;
	TransferVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void AInvestigationObj::BeginPlay()
{
	Super::BeginPlay();

}
void AInvestigationObj::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto Player = Cast<AMyPlayer>(OtherActor);

	if (Player)
	{
		Player->SetInteractObj(this);
		UE_LOG(LogTemp, Warning, TEXT("Inve Set"));
	}
}

void AInvestigationObj::NotifyActorEndOverlap(AActor* OtherActor)
{
	auto Player = Cast<AMyPlayer>(OtherActor);

	if (Player)
	{
		Player->SetInteractObj(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("Inve Set"));
	}
}

void AInvestigationObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInvestigationObj::Interact(AMyPlayer* Player)
{
	Player->SetState(LOOT);
	Player->GetAnimInst()->PlayLootMontage();
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	MyPlayer->OpenUI(UIType::INVESTIGATION);
}

