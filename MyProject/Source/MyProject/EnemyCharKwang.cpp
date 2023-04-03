// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharKwang.h"
#include "EnemyKwang.h"
#include "EnemyAnimInstance.h"
#include "MyPlayer.h"
#include "EnemyStatComponent.h"
#include "Widget_Hp.h"
#include "Components/WidgetComponent.h"
#include "MonsterSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharKwang::AEnemyCharKwang()
{
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetRelativeLocationAndRotation(
	FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	AIControllerClass = AEnemyKwang::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyCharKwang::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInst)
	{
		AnimInst->OnMontageEnded.AddDynamic(this, &AEnemyCharKwang::OnAttackMontageEnded);
		AnimInst->OnAttackHit.AddUObject(this, &AEnemyCharKwang::AttackCheck);
	}

	HpBar->InitWidget();

	auto Bar = Cast<UWidget_Hp>(HpBar->GetUserWidgetObject());
	Bar->BindWidget_Enemy(Stat);
}

void AEnemyCharKwang::BeginPlay()
{
	Super::BeginPlay();
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(Char);

	AttackTarget = MyPlayer;
	MonsterState = IDLE;
}

void AEnemyCharKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackTarget == nullptr)
		return;

	switch (MonsterState)
	{
	case IDLE:
		UpdateIdle();
		break;
	case PATROL:
		UpdatePatrol();
		break;
	case MOVING:
		UpdateMoving();
		break;
	case ATTACKREADY:
		UpdateAttack();
		break;
	case SKILL:
		break;
	case DAMAGED:
		break;
	case DIED:
		break;
	default:
		break;
	}

}

void AEnemyCharKwang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharKwang::UpdateIdle()
{
	if (GetDistanceTo(AttackTarget) < 400)
		SetState(MOVING);
	else
	{
		SetPatrolPos();
		Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PatrolPosition);
		SetState(PATROL);
	}
}

void AEnemyCharKwang::UpdatePatrol()
{
	FVector MoveDir = PatrolPosition - GetActorLocation();

	SetActorRotation(Rot);
	AddMovementInput(MoveDir);

	if (GetDistanceTo(AttackTarget) < 500)
		SetState(MOVING);

	if (ArriveToPatrolPos())
		SetState(IDLE);
}

void AEnemyCharKwang::UpdateMoving()
{
	FVector MoveDir = AttackTarget->GetActorLocation() - GetActorLocation();
	Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());

	SetActorRotation(Rot);
	AddMovementInput(MoveDir);

	if (GetDistanceTo(AttackTarget) > 1000)
		SetState(IDLE);

	if (GetDistanceTo(AttackTarget) < 170)
		SetState(ATTACKREADY);
}

void AEnemyCharKwang::UpdateAttack()
{
	SetState(ATTACK);
	Attack(AttackTarget);
}

void AEnemyCharKwang::SetPatrolPos()
{
	FNavLocation RandomLocation;
	auto SpawnerLocation = GetSpawner()->GetLocation();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	NavSystem->GetRandomPointInNavigableRadius(SpawnerLocation, 500.f, RandomLocation);
	PatrolPosition = RandomLocation.Location;
}

bool AEnemyCharKwang::ArriveToPatrolPos()
{
	float DistanceToPos = (GetActorLocation() - PatrolPosition).Size();

	if (DistanceToPos < 100)
		return true;
	else
		return false;
}





