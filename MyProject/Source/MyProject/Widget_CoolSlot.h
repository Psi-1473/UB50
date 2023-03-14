// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_CoolSlot.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidget_CoolSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaSeconds) override;
	void InitImage(class UImage* Img);
	void ChangeCoolTime();
private:
	int16 CoolTime;

	UPROPERTY(meta = (BindWidget), EditAnywhere)
		class UImage* Img_Skill;

	UPROPERTY(meta = (BindWidget), EditAnywhere)
		class UTextBlock* Txt_Cooltime;
};