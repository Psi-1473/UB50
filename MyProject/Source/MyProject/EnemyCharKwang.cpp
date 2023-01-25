// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharKwang.h"
#include "EnemyKwang.h"
#include "EnemyAnimInstance.h"

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

void AEnemyCharKwang::OnDamaged()
{
	UE_LOG(LogTemp, Log, TEXT("Damaged!!!!!"));
}

void AEnemyCharKwang::Attack()
{
	AnimInst->PlayAttackMontage();
}



void AEnemyCharKwang::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInst)
	{
		AnimInst->OnMontageEnded.AddDynamic(this, &AEnemyCharKwang::OnAttackMontageEnded);
		//AnimInst->OnAttackHit.AddUObject(this, &AMyPlayer::AttackCheck);
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

void AEnemyCharKwang::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 어택 몽타주 엔디드 함수 추가해서 IsAttacking = false 설정 및 OnAttackEnd 브로드캐스트
	// IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AEnemyCharKwang::OnAttackMontageStarted(UAnimMontage* Montage, bool bInterrupted)
{
}


