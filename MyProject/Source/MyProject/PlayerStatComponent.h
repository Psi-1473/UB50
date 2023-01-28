// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

// 델리게이트 여기다가 추가
DECLARE_MULTICAST_DELEGATE(FHpDecreased);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent();

public:
	void SetLevel(int32 NewLevel);
	void SetHp(int32 NewHp);
	void OnAttacked(float DamagedAmount);

	int32 GetLevel() { return Level; }
	int32 GetHp() { return Hp; }
	int32 GetMaxHp() { return MaxHp; }
	float GetHpRatio() { return Hp / (float)MaxHp; }
	int32 GetAttack() { return Attack; }

	FHpDecreased OnHpDecreased;


private:
	UPROPERTY(EditAnywhere, Category = Stat)
		int32 Level;
	UPROPERTY(EditAnywhere, Category = Stat)
		int32 Hp;
	UPROPERTY(EditAnywhere, Category = Stat)
		int32 MaxHp;
	UPROPERTY(EditAnywhere, Category = Stat)
		int32 Attack;

		
};
