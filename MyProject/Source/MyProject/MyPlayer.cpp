// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayer.h"
#include "MyAnimInstance.h"
#include "Monster.h"
#include "PlayerStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget_PlayerMain.h"
#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Widget_Inventory.h"
#include "Widget_InvenSlot.h"
#include "Widget_Shop.h"
#include "Projectile.h"
#include "GameFrameWork/Actor.h"
#include "MyPlayerController.h"

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
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &AMyPlayer::PopupInventory);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyPlayer::Interact);

}
void AMyPlayer::PopupInventory()
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (bOnInventory)
	{
		//Inven->RemoveFromViewport();
		//bOnInventory = false;
		//GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		GameMode->UIManager->PopupInventory(GetWorld(), Gold, EquipWeaponId, EquipArmorId, GInstance);
		bOnInventory = true;
	}
}
void AMyPlayer::Attack()
{
	if (AttackIndex >= 3)
		return;

	if (bDamaged)
		return;

	if (bOnInventory)
		return;

	bCombo = false;
	IsAttacking = true;

	AnimInst->PlayAttackMontage();
	AnimInst->JumpToSection(AttackIndex);
	AttackIndex++;
}
void AMyPlayer::EndAttack()
{
	IsAttacking = false;
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
void AMyPlayer::OnDamaged()
{
	bDamaged = true;
	IsAttacking = false;
	bCombo = false;
	AttackIndex = 0;


	AnimInst->PlayDamagedMontage();
}

void AMyPlayer::SkillR()
{
	if (SkillCoolTimes[1] > 0)
		return;

	AnimInst->PlaySkillMontage(0);
	bSkill = true;
	StartCoolDown(1);
}

void AMyPlayer::SkillQ()
{
	// 임시
	if (SkillCoolTimes[0] > 0)
		return;

	AnimInst->PlaySkillMontage(1);
	bSkill = true;
	StartCoolDown(0);
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

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector BoxVector(AttackX, AttackY, AttackZ);

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackX,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeBox(BoxVector),
		Params);

	FVector Vec = GetActorForwardVector() * AttackX;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	if (bResult && !HitResults.IsEmpty())
	{
		for (FHitResult HitResult : HitResults)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
			AMonster* Enemy = Cast<AMonster>(HitResult.GetActor());
			FDamageEvent DamageEvent;
			Enemy->OnStun(2.f);
		}
	}

	DrawDebugBox(GetWorld(), Center, BoxVector, Rotation, DrawColor, false, 2.f);
}

void AMyPlayer::StartCoolDown(int Type)
{
	switch (Type)
	{
	case 0:
		SkillCoolTimes[PLAYERSKILL_Q] = 11;
		GetWorldTimerManager().SetTimer(QTimerHandle, this, &AMyPlayer::CoolDownQ, 1.f, true);
		break;
	case 1:
		SkillCoolTimes[PLAYERSKILL_R] = 16;
		GetWorldTimerManager().SetTimer(RTimerHandle, this, &AMyPlayer::CoolDownR, 1.f, true);
		break;
	case 2:
		SkillCoolTimes[2] = 15;
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
	//
}

void AMyPlayer::CheckCoolTime(int SkillNum)
{
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

float AMyPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(Damage);
	OnDamaged();

	if (GameMode)
	{
		GameMode->UIUpdate_Hp(Stat->GetHpRatio());
	}
	return Damage;
}

void AMyPlayer::Interact()
{
	if (CanInteractNpc == nullptr)
		return;
	if (bInteract)
	{
		Conv->RemoveFromViewport();
		bInteract = false;
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		Conv = CreateWidget(GetWorld(), Conversation);

		Conv->AddToViewport();
		UWidget_Shop* Shop = Cast<UWidget_Shop>(Conv);
		Shop->CreateSlot(CanInteractNpc);
		bInteract = true;
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}

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
	auto MyInven = Cast<UWidget_Inventory>(Inven);
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	int FromId;
	int ToId;
	switch (MyInven->GetInvenType())
	{
	case 0:
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
	case 1:
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
	case 2:
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
		auto MyInven = Cast<UWidget_Inventory>(Inven);
		MyInven->ChangeGold(Value);
	}
}

void AMyPlayer::EquipWeapon(int Id, int Idx)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	auto MyInven = Cast<UWidget_Inventory>(Inven);

	if (GInstance->GetWeaponData(Id) == nullptr)
		return;

	if (EquipWeaponId != -1)
	{
		// 장착된 아이템 해제
		WeaponList[Idx] = GInstance->GetWeaponData(EquipWeaponId);
	}
	else
	{
		WeaponList[Idx] = nullptr;	
	}

	MyInven->Slots[Idx]->SetWeaponItem();
	MyInven->ChangeWeapon(Id, GInstance);
	EquipWeaponId = Id;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Weapon Equip!"));
}

void AMyPlayer::EquipArmor(int Id, int Idx)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	auto MyInven = Cast<UWidget_Inventory>(Inven);

	if (GInstance->GetArmorData(Id) == nullptr)
		return;

	if (EquipArmorId != -1)
	{
		// 장착된 아이템 해제
		ArmorList[Idx] = GInstance->GetArmorData(EquipArmorId);
	}
	else
	{
		ArmorList[Idx] = nullptr;
	}

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


