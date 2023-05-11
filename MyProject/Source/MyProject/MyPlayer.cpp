// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayer.h"
#include "MyAnimInstance.h"
#include "Monster.h"
#include "PlayerStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget_PlayerMain.h"
#include "DEFINE.H"
#include "MyGameMode.h"
#include "Manager_Scene.h"
#include "Kismet/GameplayStatics.h"
#include "Widget_Inventory.h"
#include "Widget_InvenSlot.h"
#include "Manager_Inven.h"
#include "Widget_Shop.h"
#include "Projectile.h"
#include "GameFrameWork/Actor.h"
#include "MyPlayerController.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PlayerAudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"


// Sets default values
AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	AudioPlayer = CreateDefaultSubobject<UPlayerAudioComponent>(TEXT("AudioPlayer"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	AudioPlayer->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);


	SpringArm->TargetArmLength = 700;
	Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	

	GetMesh()->SetRelativeLocationAndRotation(
	FVector(0.f, -0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Tough/Meshes/Greystone_Tough.Greystone_Tough'"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PARTICLE(TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Curse_Blink.NS_Curse_Blink'"));
	if (PARTICLE.Succeeded())
	{
		SkillREmitter = PARTICLE.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> Click(TEXT("SoundCue'/Game/Sound/UI/InvenClick.InvenClick'"));
	static ConstructorHelpers::FObjectFinder<USoundWave> Equip(TEXT("SoundWave'/Game/Sound/UI/ItemEquip.ItemEquip'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> QClear(TEXT("SoundCue'/Game/Sound/UI/QuestComplete.QuestComplete'"));


	if (Click.Succeeded())
		UICue = Click.Object;
	if (Equip.Succeeded())
		EquipCue = Equip.Object;
	if (QClear.Succeeded())
		QuestClearCue = QClear.Object;



	Stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("STAT"));
	GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	SkillCoolTimes.Init(0, 3);

}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (GameMode)
	{
		GameMode->UIUpdate_Hp(Stat->GetHpRatio());
	}
	SpawnSpot = GetActorLocation();
	UseGInstance
}

void AMyPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInst = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInst)
	{
		AnimInst->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnAttackMontageEnded);
		AnimInst->OnAttackHit.AddUObject(this, &AMyPlayer::AttackCheck);
	}

	// TODO Hp바 델리게이트 바인딩
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyPlayer::Attack()
{
	UseGInstance
	if (AttackIndex >= 3)
		return;

	if (PlayerState == DAMAGED)
		return;

	if (GInstance->InvenManager->GetOnInventory())
		return;

	bCombo = false;

	StopMoving();
	SetState(ATTACK);
	
	AnimInst->PlayAttackMontage();
	AnimInst->JumpToSection(AttackIndex);
	AttackIndex++;
}
void AMyPlayer::EndAttack()
{
	SetState(IDLE);
	bCombo = false;
	AttackIndex = 0;
}
void AMyPlayer::AttackCheck()
{
	TArray<FHitResult> HitResults;

	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 100.f;

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
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

	if (bResult && !HitResults.IsEmpty())
	{
		for(FHitResult HitResult : HitResults)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
			AMonster* Enemy = Cast<AMonster>(HitResult.GetActor());
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
		}
	}
}
float AMyPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetState() == DIED)
		return 0;

	Stat->OnAttacked(Damage);
	OnDamaged();

	if (GameMode)
	{
		GameMode->UIUpdate_Hp(Stat->GetHpRatio());
	}
	return Damage;
}

void AMyPlayer::ActivateSkill(int SkillNum)
{
	if (SkillCoolTimes[SkillNum] > 0)
		return;

	StopMoving();
	AnimInst->PlaySkillMontage(SkillNum);
	SetState(SKILL);
	StartCoolDown(SkillNum);
}

void AMyPlayer::Fire()
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
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Fire : Skill Q Fire"));
		FVector LaunchDirection = GetActorForwardVector();
		Projectile->SetPlayer(this);
		Projectile->SetIsOwnerPlayer(true);
		Projectile->FireInDirection(LaunchDirection);
	}
}

void AMyPlayer::SkillRAttackCheck()
{
	float AttackX = 250.f;
	float AttackY = 100.f;
	float AttackZ = 200.f;

	FVector Dir = GetActorForwardVector() * 100;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SkillREmitter, GetActorLocation() + Dir, GetActorRotation());
	TArray<FHitResult> HitResults = BoxHitResults(AttackX, AttackY, AttackZ, false);

	if (!HitResults.IsEmpty())
	{
		for (FHitResult HitResult : HitResults)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
			AMonster* Enemy = Cast<AMonster>(HitResult.GetActor());
			if (!Enemy)
				return;
			FDamageEvent DamageEvent;
			Enemy->OnStun(2.f);
		}
	}

}

void AMyPlayer::SkillEAttackCheck()
{
	float AttackX = 200.f;
	float AttackY = 200.f;
	float AttackZ = 200.f;

	TArray<FHitResult> HitResults = BoxHitResults(AttackX, AttackY, AttackZ, true);
	if (!HitResults.IsEmpty())
	{
		for (FHitResult HitResult : HitResults)
		{
			UE_LOG(LogTemp, Log, TEXT("E Hit Actor : %s"), *HitResult.GetActor()->GetName());
			AMonster* Enemy = Cast<AMonster>(HitResult.GetActor());
			FDamageEvent DamageEvent;
			if (Enemy == nullptr)
				return;
			if (SkillRCombo >= 3)
			{
				Enemy->OnStun(1.5f);
				SkillRCombo = 0;
			}
			else
			{
				Enemy->TakeDamage(5, DamageEvent, GetController(), this);
				SkillRCombo++;
			}


		}
	}
}

TArray<FHitResult> AMyPlayer::BoxHitResults(float X, float Y, float Z, bool IsPlayerCenter)
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	FVector BoxVector(X, Y, Z);
	FVector StartVector = GetActorLocation();
	FVector Vec = GetActorForwardVector() * X;

	if (IsPlayerCenter)
	{
		StartVector -= FVector(X / 2, 0, 0);
		Vec /= 2;
	}
	FVector EndVector = GetActorLocation() + Vec;


	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * X,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeBox(BoxVector),
		Params);

	FVector Center = GetActorLocation() + Vec * 0.5f;
	if (IsPlayerCenter) Center = GetActorLocation();
	FQuat Rotation = FRotationMatrix::MakeFromZ(GetActorForwardVector() * X).ToQuat();
	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugBox(GetWorld(), Center, BoxVector, Rotation, DrawColor, false, 2.f);

	return HitResults;
}

void AMyPlayer::StartCoolDown(int Type)
{
	switch (Type)
	{
	case PLAYERSKILL_R:
		SkillCoolTimes[PLAYERSKILL_R] = 16;
		GetWorldTimerManager().SetTimer(RTimerHandle, this, &AMyPlayer::CoolDownR, 1.f, true);
		break;
	case PLAYERSKILL_Q:
		SkillCoolTimes[PLAYERSKILL_Q] = 11;
		GetWorldTimerManager().SetTimer(QTimerHandle, this, &AMyPlayer::CoolDownQ, 1.f, true);
		break;
	case PLAYERSKILL_E:
		SkillCoolTimes[PLAYERSKILL_E] = 6;
		GetWorldTimerManager().SetTimer(ETimerHandle, this, &AMyPlayer::CoolDownE, 1.f, true);
		break;
	}
	
}

void AMyPlayer::CoolDownQ()
{
	SkillCoolTimes[PLAYERSKILL_Q]--;

	if (SkillCoolTimes[PLAYERSKILL_Q] <= 0)
	{
		SkillCoolTimes[PLAYERSKILL_Q] = 0;
		GetWorldTimerManager().ClearTimer(QTimerHandle);
	}
	else
	{
		GetWorldTimerManager().SetTimer(QTimerHandle, this, &AMyPlayer::CoolDownQ, 1.f, true);
	}

	GameMode->QSkillUpdate(SkillCoolTimes[PLAYERSKILL_Q]);
}

void AMyPlayer::CoolDownR()
{
	SkillCoolTimes[PLAYERSKILL_R]--;

	if (SkillCoolTimes[PLAYERSKILL_R] <= 0)
	{
		SkillCoolTimes[PLAYERSKILL_R] = 0;
		GetWorldTimerManager().ClearTimer(RTimerHandle);
	}
	else
	{
		GetWorldTimerManager().SetTimer(RTimerHandle, this, &AMyPlayer::CoolDownR, 1.f, true);
	}

	GameMode->RSkillUpdate(SkillCoolTimes[PLAYERSKILL_R]);
}

void AMyPlayer::CoolDownE()
{
	SkillCoolTimes[PLAYERSKILL_E]--;

	if (SkillCoolTimes[PLAYERSKILL_E] <= 0)
	{
		SkillCoolTimes[PLAYERSKILL_E] = 0;
		GetWorldTimerManager().ClearTimer(ETimerHandle);
	}
	else
	{
		GetWorldTimerManager().SetTimer(ETimerHandle, this, &AMyPlayer::CoolDownE, 1.f, true);
	}

	GameMode->ESkillUpdate(SkillCoolTimes[PLAYERSKILL_E]);
}

void AMyPlayer::OnDamaged()
{
	if (Stat->GetHp() <= 0)
	{
		SetState(DIED);
		AnimInst->PlayDeathMontage();	
		return;
	}

	if (GetState() == SKILL)
		return;

	SetState(DAMAGED);
	bCombo = false;
	AttackIndex = 0;
	AnimInst->PlayDamagedMontage();
}

void AMyPlayer::Recovery(int NumberToIncrease)
{
	Stat->SetHp(Stat->GetHp() + NumberToIncrease);
	if (GameMode)
		GameMode->UIUpdate_Hp(Stat->GetHpRatio());
}

void AMyPlayer::Respawn()
{
	SetActorLocation(SpawnSpot);
	SetState(IDLE);
}

void AMyPlayer::PlaySound(int Number)
{
	AudioPlayer->PlaySound(Number);
}

void AMyPlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//IsAttacking = false;
	//bCombo = false;
	//AttackIndex = 0;
}

void AMyPlayer::OnAttackMontageStarted(UAnimMontage* Montage, bool bInterrupted)
{
	bCombo = false;
}

void AMyPlayer::OpenUI(UIType MyUIType)
{
	UseGInstance

	PlayUiSound();
	GInstance->UIManager->PopupUI(GetWorld(), MyUIType);
	auto PlayerController = Cast<AMyPlayerController>(GetController());

	if (MyUIType == UIType::INVESTIGATION)
		return;

	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->bShowMouseCursor = true;
}

void AMyPlayer::CloseUI(UIType MyUIType)
{
	UseGInstance
	PlayUiSound();
	GInstance->UIManager->CloseUI(MyUIType);
	if (MyUIType == UIType::INVESTIGATION)
		return;
	CloseCursorInGame();

}


void AMyPlayer::StopMoving()
{
	GetCharacterMovement()->StopMovementImmediately();
}

void AMyPlayer::CloseCursorInGame()
{
	UseGInstance
	if (GInstance->UIManager->GetUiNumber() <= 0)
	{
		auto PlayerController = Cast<AMyPlayerController>(GetController());
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

void AMyPlayer::PlayUiSound()
{
	UGameplayStatics::PlaySound2D(this, UICue);
}

void AMyPlayer::PlayQuestClearSound()
{
	UGameplayStatics::PlaySound2D(this, QuestClearCue);
}


void AMyPlayer::PlayEquipSound()
{
	UGameplayStatics::PlaySound2D(this, EquipCue);
}



