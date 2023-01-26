// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsDamaged.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTDecorator_IsDamaged : public UBTDecorator
{
	GENERATED_BODY()

public:

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
};
