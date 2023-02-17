// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharKwang.h"
#include "EnemyKwang.h"
#include "EnemyAnimInstance.h"
#include "MyPlayer.h"
#include "EnemyStatComponent.h"
#include "Widget_Hp.h"
#include "Components/WidgetComponent.h"
#include "MonsterSpawner.h"

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

}

void AEnemyCharKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharKwang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}




