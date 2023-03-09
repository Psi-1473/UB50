// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsDamaged.h"
#include "EnemyKwang.h"
#include "EnemyCharKwang.h"
#include "Monster.h"


bool UBTDecorator_IsDamaged::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = UBTDecorator::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return false;

	AMonster* Pawn = Cast<AMonster>(CurrentPawn);
	bool Damaged = Pawn->GetDamaged() || Pawn->bStuned;
	return bResult && Damaged;
}
