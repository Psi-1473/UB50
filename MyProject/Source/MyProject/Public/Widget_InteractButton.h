// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Define.h"
#include "Widget_InteractButton.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_InteractButton : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void OpenShopUI();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Interact;

	UPROPERTY(EditAnywhere)
	UIType MyUiType;
};
