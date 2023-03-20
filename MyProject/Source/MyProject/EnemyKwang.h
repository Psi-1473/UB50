// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyKwang.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AEnemyKwang : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyKwang();
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaSeconds) override;
private:
	FTimerHandle TimerHandle;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardData* BlackboardData;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

};
