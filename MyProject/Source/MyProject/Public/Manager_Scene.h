// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Manager_Scene.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UManager_Scene : public UObject
{
	GENERATED_BODY()
	
public:
	void SavePlayerData(class AMyPlayer* MyPlayer);
	void LoadPlayerData(class AMyPlayer* MyPlayer);
};
