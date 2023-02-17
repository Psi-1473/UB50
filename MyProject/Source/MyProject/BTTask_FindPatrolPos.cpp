// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharKwang.h"
#include "MonsterSpawner.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	// 언리얼 툴에서 볼 수 있게 노드네임 설정
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (NavSystem == nullptr)
		return EBTNodeResult::Failed;

	FNavLocation RandomLocation;
	auto Kwang = Cast<AEnemyCharKwang>(OwnerComp.GetAIOwner()->GetPawn());
	auto SpawnerLocation = Kwang->GetSpawner()->GetLocation();


	if (NavSystem->GetRandomPointInNavigableRadius(SpawnerLocation, 500.f, RandomLocation))
	{
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
