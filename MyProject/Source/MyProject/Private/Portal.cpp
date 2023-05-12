// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/BoxComponent.h"
#include "../MyPlayer.h"
#include "../DEFINE.H"
#include "Manager_Scene.h"
#include "../MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Manager_UI.h"


// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	RootComponent = TransferVolume;
	TransferVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	AActor::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void APortal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto Player = Cast<AMyPlayer>(OtherActor);

	if (Player)
	{
		Player->SetInteractObj(this);
		PlayerInfo = Player;
		UE_LOG(LogTemp, Warning, TEXT("Portal Set"));
	}
}

void APortal::NotifyActorEndOverlap(AActor* OtherActor)
{
	auto Player = Cast<AMyPlayer>(OtherActor);

	if (Player)
	{
		Player->SetInteractObj(nullptr);
		PlayerInfo = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Portal Null"));
	}
}

void APortal::Interact(AMyPlayer* Player)
{
	PopupLoadingUI();
}

void APortal::PopupLoadingUI()
{
	if (PlayerInfo == nullptr)
		return;

	UseGInstance
	GInstance->UIManager->PopupUI(GetWorld(), UIType::LOADING);

	GetWorldTimerManager().SetTimer(NextLevelTimer, this, &APortal::MoveToOtherLevel, 2.f, true);
}

void APortal::MoveToOtherLevel()
{
	UGameplayStatics::OpenLevel(this, TransferLevelName);
}


