// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyKwang.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster.h"
#include "GameFrameWork/CharacterMovementComponent.h"

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
	// 빙의가 되었을 때 실행
	Super::OnPossess(InPawn);

	// 블랙보드 사용
	BlackboardComp = Blackboard;
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
	//빙의가 해제되었을 때 실행
	Super::OnUnPossess();

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void AEnemyKwang::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UObject* Target;
	Target = BlackboardComp->GetValueAsObject(FName(TEXT("Target")));
	if (Target == nullptr)
		return;

	//FVector PawnLocation = GetOwner()->GetActorLocation();
	FVector TargetLocation = Cast<AActor>(Target)->GetActorLocation();

	float DistanceToPlayer = (GetPawn()->GetActorLocation() - TargetLocation).Size();

	if (DistanceToPlayer > 1500)
	{
		BlackboardComp->SetValueAsObject(FName(TEXT("Target")), nullptr);
		auto OwnerPawn = Cast<AMonster>(GetPawn());
		OwnerPawn->GetCharacterMovement()->StopMovementImmediately();
	}

}
