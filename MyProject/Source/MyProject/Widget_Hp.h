// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Hp.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_Hp : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdatePlayerHp();

private:
	UPROPERTY()
	class UProgressBar* HpBar;

	class AMyPlayer* Player;

	
};
