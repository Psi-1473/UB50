// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "EnemyAnimInstance.h"
#include "MyPlayer.h"
#include "EnemyStatComponent.h"
#include "MonsterSpawner.h"
#include "Components/WidgetComponent.h"
#include "Widget_Hp.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;

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
void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInst)
	{
		AnimInst->OnMontageEnded.AddDynamic(this, &AMonster::OnAttackMontageEnded);
		AnimInst->OnAttackHit.AddUObject(this, &AMonster::AttackCheck);
	}

	HpBar->InitWidget();

	auto Bar = Cast<UWidget_Hp>(HpBar->GetUserWidgetObject());
	Bar->BindWidget_Enemy(Stat);
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

void AMonster::SetSpawner(AMonsterSpawner* Spawner)
{
	MySpawner = Spawner;
	MySpawner->UpCount();
}

void AMonster::OnDamaged()
{
	AnimInst->PlayDamagedMontage();
	IsDamaged = true;
}

void AMonster::Attack()
{
	AnimInst->PlayAttackMontage();
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

void AMonster::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Die"));
	AnimInst->PlayDeathMontage();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MySpawner->DownCount();
}


float AMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(Damage);
	OnDamaged();

	if (Stat->GetHp() == 0)
	{
		Die();
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
