// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnMonster.h"
#include "MonsterSpawner.h"
#include "Components/WidgetComponent.h"
#include "Widget_Hp.h"
#include "EnemyAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "MyPlayer.h"

ASpawnMonster::ASpawnMonster()
{
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));

	HpBar->SetupAttachment(GetMesh());
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HPBar.WBP_HPBar_C'"));

	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2d(200.f, 50.f));
		HpBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	}
}



void ASpawnMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	HpBar->InitWidget();
	
	auto Bar = Cast<UWidget_Hp>(HpBar->GetUserWidgetObject());
	Bar->BindWidget_Enemy(Stat);
}

void ASpawnMonster::SetSpawner(AMonsterSpawner* Spawner)
{
	MySpawner = Spawner;
	MySpawner->UpCount();
}
void ASpawnMonster::OnDamaged()
{
	Super::OnDamaged();
	SetState(DAMAGED);
	AnimInst->PlayDamagedMontage();
}

void ASpawnMonster::Attack(AMyPlayer* Target)
{
	if (MonsterState == DAMAGED || MonsterState == DIED)
		return;
	Super::Attack(Target);
	AnimInst->PlayAttackMontage();
}

void ASpawnMonster::Die(AMyPlayer* Player)
{
	Super::Die(Player);
	AnimInst->PlayDeathMontage();
	MySpawner->DownCount();
}

void ASpawnMonster::OnStun(float Tick)
{
	Super::OnStun(Tick);
	AnimInst->StopAllMontages(0.f);
}

void ASpawnMonster::UpdateIdle()
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

void ASpawnMonster::UpdatePatrol()
{
	FVector MoveDir = PatrolPosition - GetActorLocation();

	SetActorRotation(Rot);
	AddMovementInput(MoveDir);

	if (GetDistanceTo(AttackTarget) < 500)
		SetState(MOVING);

	if (ArriveToPatrolPos())
		SetState(IDLE);
}

void ASpawnMonster::UpdateMoving()
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

void ASpawnMonster::UpdateAttack()
{
	SetState(ATTACK);
	Attack(AttackTarget);
}

void ASpawnMonster::SetPatrolPos()
{
	FNavLocation RandomLocation;
	auto SpawnerLocation = GetSpawner()->GetLocation();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	NavSystem->GetRandomPointInNavigableRadius(SpawnerLocation, 500.f, RandomLocation);
	PatrolPosition = RandomLocation.Location;
}

bool ASpawnMonster::ArriveToPatrolPos()
{
	float DistanceToPos = (GetActorLocation() - PatrolPosition).Size();

	if (DistanceToPos < 100)
		return true;
	else
		return false;
}
