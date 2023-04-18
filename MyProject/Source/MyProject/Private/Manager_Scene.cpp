// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Scene.h"
#include "../MyGameInstance.h"
#include "../MyPlayer.h"

void UManager_Scene::SavePlayerData(AMyPlayer* MyPlayer)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(MyPlayer->GetGameInstance());

	GInstance->SetMapChanged(true);
	GInstance->SetPlayerData(MyPlayer);

	UE_LOG(LogTemp, Warning, TEXT("Save Player Data"));
}

void UManager_Scene::LoadPlayerData(AMyPlayer* MyPlayer)
{
	
}
