// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BossAnimInstance.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"



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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(TEXT("ParticleSystem'/Game/ParagonGreystone/FX/Particles/Greystone/Abilities/Ultimate/FX/P_Greystone_HToKill_Trail.P_Greystone_HToKill_Trail'"));
	if (PARTICLE.Succeeded())
	{
		Emitter = PARTICLE.Object;
	}
	//AIControllerClass = AEnemyKwang::StaticClass();
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UsableSkills.Init(true, 5);
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

	if (IsAttacking)
		return;

	// ½ºÅ³ »ç¿ë °¡´É?
	if (CanUseSkill())
	{
		UseSkill(PickUsableSkill());
	}
	else
	{
		FVector MoveDir = AttackTarget->GetActorLocation() - GetActorLocation();
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());

		if (GetDistanceTo(AttackTarget) > 300 && IsAttacking == false)
		{
			SetActorRotation(Rot);
			AddMovementInput(MoveDir);
		}
		else
		{
			Attack(AttackTarget);
		}
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
	if (CanSkills == false)
		return false;

	else return true;
}

int ABossMonster::PickUsableSkill()
{
	TArray<int> SkillIndex;
	for (int i = 0; i < 5; i++)
	{
		if (i == 0)
			continue;

		if (UsableSkills[i] == true)
		{
			SkillIndex.Add(i);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Skill ADD"));
		}
	}
	
	if (SkillIndex.Num() == 0)
		return 0;


	int RandIndex = FMath::RandRange(0, SkillIndex.Num() - 1);
	int RetIndex = SkillIndex[RandIndex];
	UE_LOG(LogTemp, Warning, TEXT("Return Index : %d"), RetIndex);
	return RetIndex;
}

void ABossMonster::UseSkill(int SkillNum)
{
	switch (SkillNum)
	{
	case 1:
		Skill1();
		break;
	case 2:
		Skill2();
		break;
	case 3:
		Skill3();
		break;
	case 4:
		Skill4();
		break;
	}
}

void ABossMonster::Skill1()
{
	IsAttacking = true;
	AnimInst->PlayAttackMontage(1);
	UsableSkills[1] = false;
	CanSkills = false;
	GetWorldTimerManager().SetTimer(Skill1TimerHandle, this, &ABossMonster::CoolTimeZero1, BossCoolTime::SKILL1, true);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ABossMonster::ActionCoolTimeZero, BossCoolTime::ALLSKILL, true);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill 1"));
}

void ABossMonster::Skill2()
{
	IsAttacking = true;
	AnimInst->PlayAttackMontage(2);
	UsableSkills[2] = false;
	CanSkills = false;
	GetWorldTimerManager().SetTimer(Skill2TimerHandle, this, &ABossMonster::CoolTimeZero2, BossCoolTime::SKILL2, true);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ABossMonster::ActionCoolTimeZero, BossCoolTime::ALLSKILL, true);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill 2"));
}

void ABossMonster::Skill3()
{
	IsAttacking = true;
	AnimInst->PlayAttackMontage(3);
	UsableSkills[3] = false;
	CanSkills = false;
	GetWorldTimerManager().SetTimer(Skill3TimerHandle, this, &ABossMonster::CoolTimeZero3, BossCoolTime::SKILL3, true);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ABossMonster::ActionCoolTimeZero, BossCoolTime::ALLSKILL, true);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill 3"));
}

void ABossMonster::Skill4()
{
	IsAttacking = true;
	AnimInst->PlayAttackMontage(4);
	UsableSkills[4] = false;
	CanSkills = false;
	GetWorldTimerManager().SetTimer(Skill4TimerHandle, this, &ABossMonster::CoolTimeZero4, BossCoolTime::SKILL4, true);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ABossMonster::ActionCoolTimeZero, BossCoolTime::ALLSKILL, true);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill 4"));
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

void ABossMonster::Skill3Targeting()
{
	FVector TargetPlace;

	TargetPlace = AttackTarget->GetTargetLocation();


	// ÁÂÇ¥¿¡ °ð ¹º°¡ ¶³¾îÁø´Ù´Â ÀÌÆåÆ® ¹ß»ý ½ÃÅ°±â
	
	//

	Skill3Fire(TargetPlace);
}

void ABossMonster::Skill3Fire(FVector Transform)
{
	const FTransform Trans(Transform);


	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter, Trans);
	float AttackX = 100.f;
	float AttackY = 100.f;
	float AttackZ = 100.f;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector BoxVector(AttackX, AttackY, AttackZ);

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		Transform,
		Transform + GetActorForwardVector() * AttackX,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(BoxVector),
		Params);

	FVector Vec = GetActorForwardVector() * AttackX;
	FVector Center = Transform + Vec * 0.5f;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	if (bResult)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
		AMyPlayer* HitPlayer = Cast<AMyPlayer>(HitResult.GetActor());
		FDamageEvent DamageEvent;
		//HitPlayer->OnStun(2.f);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player Hit"));
	}

	DrawDebugBox(GetWorld(), Center, BoxVector, Rotation, DrawColor, false, 2.f);


}

void ABossMonster::CoolTimeZero1()
{
	UsableSkills[1] = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("1 CoolTime Reset"));
}

void ABossMonster::CoolTimeZero2()
{
	UsableSkills[2] = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("2 CoolTime Reset"));
}

void ABossMonster::CoolTimeZero3()
{
	UsableSkills[3] = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("3 CoolTime Reset"));
}

void ABossMonster::CoolTimeZero4()
{
	UsableSkills[4] = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("4 CoolTime Reset"));
}

void ABossMonster::ActionCoolTimeZero()
{
	CanSkills = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Action CoolTime Reset"));
}
