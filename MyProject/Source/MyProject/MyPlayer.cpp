// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayer.h"
#include "MyAnimInstance.h"
#include "Monster.h"
#include "PlayerStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget_PlayerMain.h"
#include "DEFINE.H"
#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Widget_Inventory.h"
#include "Widget_InvenSlot.h"
#include "Widget_Shop.h"
#include "Projectile.h"
#include "GameFrameWork/Actor.h"
#include "MyPlayerController.h"
#include "GameFrameWork/CharacterMovementComponent.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
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

	Stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("STAT"));
	GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> IW(TEXT("WidgetBlueprint'/Game/UI/WBP_Inventory.WBP_Inventory_C'"));
	if (IW.Succeeded())
	{
		Inventory = IW.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> CW(TEXT("WidgetBlueprint'/Game/UI/WBP_Shop.WBP_Shop_C'"));
	if (IW.Succeeded())
	{
		Conversation = CW.Class;
	}


	WeaponList.Init(nullptr, 24);
	ArmorList.Init(nullptr, 24);
	UseItemList.Init(nullptr, 24);
	SkillCoolTimes.Init(0, 3);

}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (GameMode)
	{
		GameMode->UIUpdate_Hp(Stat->GetHpRatio());
	}
	Gold = 1000;
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
	if (AttackIndex >= 3)
		return;

	if (PlayerState == DAMAGED)
		return;

	if (bOnInventory)
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
	Stat->OnAttacked(Damage);
	OnDamaged();
	SetState(DAMAGED);
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
	float AttackX = 1000.f;
	float AttackY = 1000.f;
	float AttackZ = 1000.f;

	TArray<FHitResult> HitResults = BoxHitResults(AttackX, AttackY, AttackZ, true);
	if (!HitResults.IsEmpty())
	{
		for (FHitResult HitResult : HitResults)
		{
			UE_LOG(LogTemp, Log, TEXT("E Hit Actor : %s"), *HitResult.GetActor()->GetName());
			AMonster* Enemy = Cast<AMonster>(HitResult.GetActor());
			FDamageEvent DamageEvent;
			Enemy->OnStun(2.f);
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

	GameMode->RSkillUpdate(SkillCoolTimes[PLAYERSKILL_R]);
}

void AMyPlayer::OnDamaged()
{
	SetState(DAMAGED);
	bCombo = false;
	AttackIndex = 0;

	AnimInst->PlayDamagedMontage();
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
	GameMode->UIManager->PopupUI(GetWorld(), MyUIType);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void AMyPlayer::CloseUI(UIType MyUIType)
{
	GameMode->UIManager->CloseUI(MyUIType);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void AMyPlayer::EquipWeapon(int Id, int Idx)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	auto MyInven = Cast<UWidget_Inventory>(GameMode->UIManager->GetInven());

	if (GInstance->GetWeaponData(Id) == nullptr)
		return;

	if (EquipWeaponId != -1)
		WeaponList[Idx] = GInstance->GetWeaponData(EquipWeaponId);
	else
		WeaponList[Idx] = nullptr;

	MyInven->Slots[Idx]->SetWeaponItem();
	MyInven->ChangeWeapon(Id, GInstance);
	EquipWeaponId = Id;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Weapon Equip!"));
}

void AMyPlayer::EquipArmor(int Id, int Idx)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	auto MyInven = Cast<UWidget_Inventory>(GameMode->UIManager->GetInven());

	if (GInstance->GetArmorData(Id) == nullptr)
		return;

	if (EquipArmorId != -1)
		ArmorList[Idx] = GInstance->GetArmorData(EquipArmorId);
	else
		ArmorList[Idx] = nullptr;

	MyInven->Slots[Idx]->SetArmorItem();
	MyInven->ChangeArmor(Id, GInstance);
	EquipArmorId = Id;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Armor Equip!"));
}

void AMyPlayer::UseItem(int Id, int Idx)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GInstance->GetUseData(Id) == nullptr)
		return;


	// 아이템 정보를 받아와서 
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("item Use!"));
}

void AMyPlayer::AddItem(int id, int ItemType)
{
}

void AMyPlayer::AddItemWeapon(int Id)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	int Idx = FindNextInvenIndex(WEAPON);
	WeaponList[Idx] = GInstance->GetWeaponData(Id);
	if (bOnInventory)
		GameMode->UIManager->UpdateInventory(FindNextInvenIndex(WEAPON));
}

void AMyPlayer::AddItemArmor(int Id)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	int Idx = FindNextInvenIndex(ARMOR);
	ArmorList[Idx] = GInstance->GetArmorData(Id);
	if (bOnInventory)
		GameMode->UIManager->UpdateInventory(FindNextInvenIndex(ARMOR));
}

void AMyPlayer::AddItemUse(int Id)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	int Idx = FindNextInvenIndex(USEITEM);
	UseItemList[Idx] = GInstance->GetUseData(Id);
	if (bOnInventory)
		GameMode->UIManager->UpdateInventory(FindNextInvenIndex(USEITEM));

	GameMode->QuestManager->AddQuestTargetNum("Collect", Id);
}

int AMyPlayer::FindNextInvenIndex(int ItemType)
{
	// 인벤토리가 가득 찼을 때 처리.
	for (int i = 0; i < INVENTORYNUMBER; i++)
	{
		switch (ItemType)
		{
		case WEAPON:
			if (WeaponList[i] == nullptr)
				return i;
			break;
		case ARMOR:
			if (ArmorList[i] == nullptr)
				return i;
			break;
		case USEITEM:
			if (UseItemList[i] == nullptr)
				return i;
			break;
		}
	}

	return -1;
}

bool AMyPlayer::DraggingSwap(int from, int to)
{
	auto MyInven = Cast<UWidget_Inventory>(GameMode->UIManager->GetInven());
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	int FromId;
	int ToId;

	switch (MyInven->GetInvenType())
	{
	case WEAPON:
		FromId = WeaponList[from]->Id;

		if (WeaponList[to] == nullptr)
		{
			WeaponList[from] = nullptr;
			WeaponIndex = from;
		}
		else
		{
			ToId = WeaponList[to]->Id;
			WeaponList[from] = GInstance->GetWeaponData(ToId);
		}
		WeaponList[to] = GInstance->GetWeaponData(FromId);
		MyInven->Slots[to]->SetWeaponItem();
		MyInven->Slots[from]->SetWeaponItem();

		break;
	case ARMOR:
		FromId = ArmorList[from]->Id;
		if (ArmorList[to] == nullptr)
		{
			ArmorList[from] = nullptr;
			ArmorIndex = from;
		}
		else
		{
			ToId = ArmorList[to]->Id;
			ArmorList[from] = GInstance->GetArmorData(ToId);
		}
		ArmorList[to] = GInstance->GetArmorData(FromId);
		MyInven->Slots[from]->SetArmorItem();
		MyInven->Slots[to]->SetArmorItem();
		break;
	case USEITEM:
		FromId = UseItemList[from]->Id;

		if (UseItemList[to] == nullptr)
		{
			UseItemList[from] = nullptr;
			UseItemIndex = from;
		}
		else
		{
			ToId = UseItemList[to]->Id;
			UseItemList[from] = GInstance->GetUseData(ToId);
		}
		UseItemList[to] = GInstance->GetUseData(FromId);
		MyInven->Slots[from]->SetUseItem();
		MyInven->Slots[to]->SetUseItem();
		break;
	}

	return true;
}

void AMyPlayer::ChangeGold(int Value)
{
	Gold += Value;

	if (bOnInventory)
	{
		GameMode->UIManager->ChangeInvenGold(Value);
	}
}

void AMyPlayer::StopMoving()
{
	GetCharacterMovement()->StopMovementImmediately();
}

