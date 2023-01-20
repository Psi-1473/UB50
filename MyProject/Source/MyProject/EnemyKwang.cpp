// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyKwang.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyKwang::AEnemyKwang()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BD_Enemy.BD_Enemy'"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}
	
}

void AEnemyKwang::OnPossess(APawn* InPawn)
{
	// ���ǰ� �Ǿ��� �� ����
	Super::OnPossess(InPawn);

	// Ÿ�̸� ���� - Ÿ�̸� �ڵ��� �̿��� 3�ʸ��� 1���� �������� ����
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyKwang::RandomMove, 3.f, true);

	// ������ ���
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BlackboardData, BlackboardComp))
	{
		if (RunBehaviorTree(BehaviorTree))
		{
			// To do
		}
	}


}

void AEnemyKwang::OnUnPossess()
{
	//���ǰ� �����Ǿ��� �� ����
	Super::OnUnPossess();

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void AEnemyKwang::RandomMove()
{
	auto CurrentPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (NavSystem == nullptr)
		return;

	FNavLocation RandomLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, RandomLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
	}
}
