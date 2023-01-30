// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FHpDecreased);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
public:
	void SetLevel(int32 NewLevel);
	void SetHp(float NewHp);
	void OnAttacked(float DamagedAmount);

	FHpDecreased OnHpDecreased;

public:
	FString GetName() { return Name; }
	int32 GetLevel() { return Level; }
	int32 GetHp() { return Hp; }
	int32 GetAttack() { return Attack; }
	int32 GetSpeed() { return Speed; }
	float GetHpRatio() { return Hp / (float)MaxHp; }

private:
	UPROPERTY(EditAnywhere, Category = Stat)
	FString Name;

	UPROPERTY(EditAnywhere, Category = Stat)
	int32 Level;

	UPROPERTY(EditAnywhere, Category = Stat)
	int32 Attack;

	UPROPERTY(EditAnywhere, Category = Stat)
	int32 Hp;

	UPROPERTY(EditAnywhere, Category = Stat)
	int32 MaxHp;

	UPROPERTY(EditAnywhere, Category = Stat)
	int32 Speed;
		
};
