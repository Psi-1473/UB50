// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);
	void OnSprint();
	void OffSprint();
	void ClickAttack();
	void ClickRSkill();
	void ClickQSkill();
	void ClickESkill();
	void PopupInventory();
	void PopupQuest();
	void Interact();

	void Click1();
	void Click2();
	void Click3();
	void Click4();
	void Click5();
	//void PopupInventory();

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY();
	class AMyPlayer* MyPlayer;

};
// 현재 플레이어의 TakeDamage함수 일단 지워놓음
// id 바뀌는지 확인하자