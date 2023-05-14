// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "EnemyAnimInstance.h"
#include "MyPlayer.h"
#include "MyGameMode.h"
#include "MyGameInstance.h"
#include "Manager_Quest.h"
#include "Manager_Inven.h"
#include "EnemyStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget_Hp.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("STAT"));

}
void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	AttackTarget = MyPlayer;
}

// Called every frame
void AMonster::Tick(float DeltaTime)
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


void AMonster::OnDamaged()
{	
	GetCharacterMovement()->StopMovementImmediately();
}

void AMonster::Attack(AMyPlayer* Target)
{
	if (IsDied)
		return;

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	SetActorRotation(Rot);
}

void AMonster::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 2.f);



	if (bResult && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
		AMyPlayer* Player = Cast<AMyPlayer>(HitResult.GetActor());

		if (Player)
		{
			FDamageEvent DamageEvent;
			Player->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
		}
	}
}

void AMonster::Die(AMyPlayer* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Die"));
	IsDied = true;
	
	SetState(DIED);
	UseGameMode
	UseGInstance
	GInstance->QuestManager->AddQuestTargetNum("Hunting", Stat->GetId());
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DropItemOrGold(Player);
}

void AMonster::DropItemOrGold(AMyPlayer* Player)
{
	UseGInstance
	int32 MaxGold = Stat->GetMaxGold();
	int32 MinGold = Stat->GetMinGold();

	int32 RandGold = FMath::RandRange(MinGold, MaxGold);

	GInstance->InvenManager->ChangeGold(GInstance, RandGold);
}

void AMonster::OnStun(float Tick)
{
	SetState(STUNED);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AMonster::OffStun, Tick, true);
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	GetCharacterMovement()->StopMovementImmediately();
	
}

void AMonster::OffStun()
{
	SetState(IDLE);
}


float AMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(Damage);
	OnDamaged();
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	if (Stat->GetHp() == 0)
	{
		AMyPlayer* Player = Cast<AMyPlayer>(DamageCauser);
		Die(Player);
	}

	return Damage;
}

void AMonster::UpdateIdle()
{
}

void AMonster::UpdatePatrol()
{
}

void AMonster::UpdateMoving()
{
}

void AMonster::UpdateAttack()
{
}

void AMonster::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 어택 몽타주 엔디드 함수 추가해서 IsAttacking = false 설정 및 OnAttackEnd 브로드캐스트
	// IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AMonster::OnAttackMontageStarted(UAnimMontage* Montage, bool bInterrupted)
{

}
