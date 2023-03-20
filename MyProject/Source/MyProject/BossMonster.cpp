// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BossAnimInstance.h"


ABossMonster::ABossMonster()
{
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	//AIControllerClass = AEnemyKwang::StaticClass();
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABossMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInst = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());;
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	Target = MyPlayer;
	
	
}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 스킬 사용 가능?

	
	FVector MoveDir = Target->GetActorLocation() - GetActorLocation();
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());

	if (GetDistanceTo(Target) > 300)
	{
		SetActorRotation(Rot);
		AddMovementInput(MoveDir);
	}
}

void ABossMonster::OnDamaged()
{
}

//void ABossMonster::Attack(AMyPlayer* Target)
//{
//}
//
//void ABossMonster::Die(AMyPlayer* Player)
//{
//}

bool ABossMonster::CanUseSkill()
{
	if (ActionCoolTime > 0)
		return false;

	else return true;
}

int ABossMonster::PickUsableSkill()
{
	return 0;
}
