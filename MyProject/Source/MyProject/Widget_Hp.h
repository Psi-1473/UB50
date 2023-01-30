// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStatComponent.h"
#include "PlayerStatComponent.h"
#include "Widget_Hp.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_Hp : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindWidget_Player(UPlayerStatComponent* Stat);
	void BindWidget_Enemy(UEnemyStatComponent* Stat);
	void UpdateHp();

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;

	UPROPERTY()
	bool IsPlayer;

	UPROPERTY()
	TWeakObjectPtr<UPlayerStatComponent> PlayerStat;
	TWeakObjectPtr<UEnemyStatComponent> EnemyStat;
	
};
