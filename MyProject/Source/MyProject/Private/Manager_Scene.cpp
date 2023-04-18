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
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(MyPlayer->GetGameInstance());

	GInstance->SetPlayerData(MyPlayer);
	if (GInstance->GetMapChanged() == false)
		return;
	GInstance->SetMapChanged(false);

	PlayerData* P = GInstance->GetPlayerData();

	for (int i = 0; i < 24; i++)
	{
		MyPlayer->WeaponList[i] = P->WData[i];
		MyPlayer->ArmorList[i] = P->AData[i];
		MyPlayer->UseItemList[i] = P->UData[i];
	}

	MyPlayer->WeaponIndex = P->WIndex;
	MyPlayer->ArmorIndex = P->AIndex;
	MyPlayer->UseItemIndex = P->UIndex;

	MyPlayer->Gold = P->Gold;
	UE_LOG(LogTemp, Warning, TEXT("Load Player Data"));
}
