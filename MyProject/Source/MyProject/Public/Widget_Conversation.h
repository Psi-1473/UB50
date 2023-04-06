// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Conversation.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_Conversation : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;


private:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* VBox_Button;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Name;

	class ANpc* OwnerNpc;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BP_Button;
	UUserWidget* InteractButton;
};
