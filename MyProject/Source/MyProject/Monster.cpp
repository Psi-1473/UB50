// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "EnemyAnimInstance.h"
#include "MyPlayer.h"
#include "EnemyStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget_Hp.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMonster::OnDamaged()
{
	
	GetCharacterMovement()->StopMovementImmediately();
	IsDamaged = true;
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
		ECollisionChannel::ECC_GameTraceChannel2,
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
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DropItemOrGold(Player);
}

void AMonster::DropItemOrGold(AMyPlayer* Player)
{
	int32 MaxGold = Stat->GetMaxGold();
	int32 MinGold = Stat->GetMinGold();

	int32 RandGold = FMath::RandRange(MinGold, MaxGold);

	Player->ChangeGold(RandGold);
}

void AMonster::OnStun(float Tick)
{
	bStuned = true;
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AMonster::OffStun, Tick, true);
	
	GetCharacterMovement()->StopMovementImmediately();
	
}

void AMonster::OffStun()
{
	bStuned = false;
}



float AMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(Damage);
	OnDamaged();

	if (Stat->GetHp() == 0)
	{
		AMyPlayer* Player = Cast<AMyPlayer>(DamageCauser);
		Die(Player);
	}

	return Damage;
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
