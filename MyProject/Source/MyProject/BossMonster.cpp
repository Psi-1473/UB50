// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BossAnimInstance.h"
#include "Projectile.h"


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

	AttackTarget = MyPlayer;
	
	
}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 스킬 사용 가능?

	
	FVector MoveDir = AttackTarget->GetActorLocation() - GetActorLocation();
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());

	if (GetDistanceTo(AttackTarget) > 300 && IsAttacking == false)
	{
		SetActorRotation(Rot);
		AddMovementInput(MoveDir);
	}
	else
	{
		//Attack(AttackTarget);
		Skill1();
	}
}

void ABossMonster::OnDamaged()
{
}

void ABossMonster::Attack(AMyPlayer* Target)
{
	if (IsAttacking)
		return;

	Super::Attack(Target);
	IsAttacking = true;
	AnimInst->PlayAttackMontage(0);
}

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

void ABossMonster::UseSkill(int SkillNum)
{
	switch (SkillNum)
	{
	case 0:
		Skill1();
		break;
	case 1:
		Skill2();
		break;
	case 2:
		Skill3();
		break;
	case 3:
		Skill4();
		break;
	}
}

void ABossMonster::Skill1()
{
	IsAttacking = true;
	AnimInst->PlayAttackMontage(1);
}

void ABossMonster::Skill2()
{
	AnimInst->PlayAttackMontage(2);
}

void ABossMonster::Skill3()
{
	AnimInst->PlayAttackMontage(3);
}

void ABossMonster::Skill4()
{
	AnimInst->PlayAttackMontage(4);
}

void ABossMonster::Skill1Fire()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, CameraRotation, SpawnParams);

	if (Projectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Boss : Skill Fire"));
		FVector LaunchDirection = GetActorForwardVector();
		Projectile->SetIsOwnerPlayer(false);
		Projectile->FireInDirection(LaunchDirection);
	}
}
