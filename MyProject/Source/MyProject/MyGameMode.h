// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPlayer.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();
	virtual void BeginPlay() override;
	void UIUpdate_Hp(float Value);

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> HUD_Class;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* CurrentWidget;
};
