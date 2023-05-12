// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BossAnimInstance.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyStatComponent.h"
#include "NavigationSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MyGameInstance.h"
#include "Manager_UI.h"
#include "Widget_Hp.h"




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
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PARTICLE2ACT(TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Vampire.NS_Dash_Vampire'"));
	if (PARTICLE2ACT.Succeeded())
	{
		Skill2ActEmitter = PARTICLE2ACT.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE3(TEXT("ParticleSystem'/Game/ParagonGreystone/FX/Particles/Greystone/Abilities/Ultimate/FX/P_Greystone_HToKill_Trail.P_Greystone_HToKill_Trail'"));
	if (PARTICLE3.Succeeded())
	{
		Skill3Emitter = PARTICLE3.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE3TARGET(TEXT("ParticleSystem'/Game/ParagonGreystone/FX/Particles/Greystone/Abilities/LeapAOE/FX/P_Greystone_LeapAOE_Start.P_Greystone_LeapAOE_Start'"));
	if (PARTICLE3.Succeeded())
	{
		Skill3TargetEmitter = PARTICLE3TARGET.Object;
	}
	
	UsableSkills.Init(true, 5);
}

void ABossMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Stat->SetMonster(TEXT("Sevarog"));
	AnimInst = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());

	
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();
	SetState(START);
	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto MyPlayer = Cast<AMyPlayer>(Char);

	UseGInstance
	UUserWidget* BossWidget = GInstance->UIManager->PopupUI(GetWorld(), UIType::BOSSHP);

	AnimInst->PlayStartMontage();
	UWidget_Hp* HpWidget = Cast<UWidget_Hp>(BossWidget);
	HpWidget->BindWidget_Enemy(Stat);
	

	AttackTarget = MyPlayer;
}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossMonster::OnDamaged()
{
	Super::OnDamaged();

	if (!CanBeStiffed)
		return;

	SetState(DAMAGED);
	AnimInst->StopAllMontages(0.f);
	AnimInst->PlayDamagedMontage();
}

void ABossMonster::Attack(AMyPlayer* Target)
{
	Super::Attack(Target);
	CanBeStiffed = false;
	AnimInst->PlayAttackMontage(0);
}

void ABossMonster::Die(AMyPlayer* Player)
{
	Super::Die(Player);
	AnimInst->PlayDeathMontage();
}

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
	SetState(SKILL);
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
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
	SetActorRotation(Rot);
	AnimInst->PlayAttackMontage(1);
	UsableSkills[1] = false;
	CanSkills = false;
	CanBeStiffed = false;
	GetWorldTimerManager().SetTimer(Skill1TimerHandle, this, &ABossMonster::CoolTimeZero1, BossCoolTime::SKILL1, true);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ABossMonster::ActionCoolTimeZero, BossCoolTime::ALLSKILL, true);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill 1"));
}

void ABossMonster::Skill2()
{
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
	SetActorRotation(Rot);
	AnimInst->PlayAttackMontage(2);
	UsableSkills[2] = false;
	CanSkills = false;
	CanBeStiffed = false;
	GetWorldTimerManager().SetTimer(Skill2TimerHandle, this, &ABossMonster::CoolTimeZero2, BossCoolTime::SKILL2, true);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ABossMonster::ActionCoolTimeZero, BossCoolTime::ALLSKILL, true);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill 2"));
}

void ABossMonster::Skill3()
{
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
	SetActorRotation(Rot);
	AnimInst->PlayAttackMontage(3);
	UsableSkills[3] = false;
	CanSkills = false;
	CanBeStiffed = false;
	GetWorldTimerManager().SetTimer(Skill3TimerHandle, this, &ABossMonster::CoolTimeZero3, BossCoolTime::SKILL3, true);
	GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &ABossMonster::ActionCoolTimeZero, BossCoolTime::ALLSKILL, true);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill 3"));
}

void ABossMonster::Skill4()
{
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
	SetActorRotation(Rot);
	AnimInst->PlayAttackMontage(4);
	UsableSkills[4] = false;
	CanSkills = false;
	CanBeStiffed = false;
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
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Skill2ActEmitter, GetActorLocation(), GetActorRotation());
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
	Skill3Transform.Empty();
	FVector TargetPlace = AttackTarget->GetActorLocation();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation RandomLocation;

	for (int i = 0; i < 5; i++)
	{
		NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 500.f, RandomLocation);
		RandomLocation.Location.Z += 50;
		Skill3Transform.Add(RandomLocation.Location);
	}
}

void ABossMonster::Skill3Fire()
{
	float AttackX = 100.f;
	float AttackY = 100.f;
	float AttackZ = 100.f;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector BoxVector(AttackX, AttackY, AttackZ);

	for (int i = 0; i < 5; i++)
	{
		FVector SkillLocation = Skill3Transform.Pop();
		FTransform Trans(SkillLocation);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill3Emitter, Trans);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill go"));
		bool bResult = GetWorld()->SweepSingleByChannel(
			OUT HitResult,
			SkillLocation,
			SkillLocation + GetActorForwardVector() * AttackX,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel6,
			FCollisionShape::MakeBox(BoxVector),
			Params);

		FVector Vec = GetActorForwardVector() * AttackX;
		FVector Center = SkillLocation + Vec * 0.5f;
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

void ABossMonster::UpdateIdle()
{
	CanBeStiffed = true;
	if (AttackTarget == nullptr)
		return;
	
	if (CanUseSkill())
	{
		UseSkill(PickUsableSkill());
	}
	else
		SetState(MOVING);

}

void ABossMonster::UpdateMoving()
{
	FVector MoveDir = AttackTarget->GetActorLocation() - GetActorLocation();
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());

	if (CanUseSkill())
	{
		UseSkill(PickUsableSkill());
		return;
	}

	SetActorRotation(Rot);
	AddMovementInput(MoveDir);

	if (GetDistanceTo(AttackTarget) < 170)
		SetState(ATTACKREADY);
}

void ABossMonster::UpdateAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("ATTACK READY"));
	SetState(ATTACK);
	Attack(AttackTarget);
}

AProjectile* ABossMonster::CreateProjectile(TSubclassOf<class AProjectile> ClassOfProjectile, FVector Muzzle, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();


	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ClassOfProjectile, Muzzle, Rot, SpawnParams);
	return Projectile;
}

