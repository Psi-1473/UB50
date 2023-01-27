// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharKwang.h"
#include "EnemyKwang.h"
#include "EnemyAnimInstance.h"
#include "MyPlayer.h"
#include "EnemyStatComponent.h"

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

	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("STAT"));
}

void AEnemyCharKwang::OnDamaged()
{
	AnimInst->PlayDamagedMontage();
	IsDamaged = true;
}

void AEnemyCharKwang::Attack()
{
	AnimInst->PlayAttackMontage();
}

void AEnemyCharKwang::AttackCheck()
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

		FDamageEvent DamageEvent;
		Player->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
		// 10은 임시
	}
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
}

void AEnemyCharKwang::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharKwang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharKwang::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(Damage);
	OnDamaged();
	return Damage;
}

void AEnemyCharKwang::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 어택 몽타주 엔디드 함수 추가해서 IsAttacking = false 설정 및 OnAttackEnd 브로드캐스트
	// IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AEnemyCharKwang::OnAttackMontageStarted(UAnimMontage* Montage, bool bInterrupted)
{
}


