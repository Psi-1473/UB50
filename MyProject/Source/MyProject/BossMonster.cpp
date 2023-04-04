// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BossAnimInstance.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyStatComponent.h"
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE2(TEXT("ParticleSystem'/Game/ParagonGreystone/FX/Particles/Greystone/Skins/Novaborn/P_Greystone_Novaborn_UltimateActive.P_Greystone_Novaborn_UltimateActive'"));
	if (PARTICLE2.Succeeded())
	{
		Skill2Emitter = PARTICLE2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE3(TEXT("ParticleSystem'/Game/ParagonGreystone/FX/Particles/Greystone/Abilities/Ultimate/FX/P_Greystone_HToKill_Trail.P_Greystone_HToKill_Trail'"));
	if (PARTICLE3.Succeeded())
	{
		Skill3Emitter = PARTICLE3.Object;
	}

	
	Stat->SetMonster("Sevarog");
	UsableSkills.Init(true, 5);
}

void ABossMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInst = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());


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
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
	SetActorRotation(Rot);
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
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
	SetActorRotation(Rot);
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
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
	SetActorRotation(Rot);
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
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
	SetActorRotation(Rot);
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

void ABossMonster::Skill2Ready()
{
	FVector EmitPos = GetActorLocation() + GetActorForwardVector() * 100;
	const FTransform Trans(EmitPos);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill2Emitter, Trans);
}

void ABossMonster::Skill2Fire()
{
	float AttackX = 100.f;
	float AttackY = 100.f;
	float AttackZ = 500.f;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector BoxVector(AttackX, AttackY, AttackZ);

	FVector StartVector = GetActorLocation() + GetActorForwardVector() * 200;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		StartVector,
		StartVector + GetActorForwardVector() * AttackZ,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeBox(BoxVector),
		Params);

	FVector Vec = GetActorForwardVector() * AttackZ;
	FVector Center = StartVector + Vec * 0.5f;
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
		if(HitPlayer)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player Hit"));
	}

	DrawDebugBox(GetWorld(), Center, BoxVector, Rotation, DrawColor, false, 2.f);
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


	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill3Emitter, Trans);
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
		ECollisionChannel::ECC_GameTraceChannel6,
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
		AMyPlayer* HitPlayer = Cast<AMyPlayer>(HitResult.GetActor());
		if (HitPlayer)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player Hit"));
		//FDamageEvent DamageEvent;
		////HitPlayer->OnStun(2.f);
	}

	DrawDebugBox(GetWorld(), Center, BoxVector, Rotation, DrawColor, false, 2.f);


}

void ABossMonster::Skill4Charge()
{
	if (Projectiles.Num() == 0)
		MuzzleLocation4 = GetActorLocation() + FVector(50, -70, 170);
	else if (Projectiles.Num() < 3)
		MuzzleLocation4 += FVector(-50, 40, 30);
	else
		MuzzleLocation4 += FVector(-50, -40, -30);

	if (Projectiles.Num() == 5)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProjectileTimer);
		return;
	}
	else
	{
		Projectiles.Add(CreateProjectile(Skill4ProjectileClass, MuzzleLocation4, GetActorRotation()));
		GetWorld()->GetTimerManager().SetTimer(ProjectileTimer, this, &ABossMonster::Skill4Charge, 0.1f, true);
	}
}

void ABossMonster::Skill4Fire()
{
	if (Projectiles.Num() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProjectileTimer);
		return;
	}
	FVector Dir = AttackTarget->GetActorLocation() - Projectiles[0]->GetActorLocation();
	Dir.Normalize();
	AProjectile* GoProjectile = Projectiles.Pop();
	GoProjectile->FireAndDestroy(Dir, 4000, 1.f);

	GetWorld()->GetTimerManager().SetTimer(ProjectileTimer, this, &ABossMonster::Skill4Fire, 0.1f, true);
}

void ABossMonster::SkillCoolTimeZero(BossSkill SkillType)
{
	UsableSkills[SkillType] = true;
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

AProjectile* ABossMonster::CreateProjectile(TSubclassOf<class AProjectile> ClassOfProjectile, FVector Muzzle, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();


	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ClassOfProjectile, Muzzle, Rot, SpawnParams);
	return Projectile;
}

