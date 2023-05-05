// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPlayer.h"
#include "Manager_UI.h"
#include "Manager_Quest.h"
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
	~AMyGameMode();
	virtual void BeginPlay() override;
	void UIUpdate_Hp(float Value);
	void QSkillUpdate(int Value);
	void RSkillUpdate(int Value);
	void ESkillUpdate(int Value);

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> HUD_Class;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* CurrentWidget;


};
