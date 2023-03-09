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
		Inven->RemoveFromViewport();
		bOnInventory = false;
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		Inven = CreateWidget(GetWorld(), Inventory);
		
		UE_LOG(LogTemp, Warning, TEXT("GameMode CurrentWidget Succeeded!"));
		Inven->AddToViewport();
		auto WInven = Cast<UWidget_Inventory>(Inven);
		WInven->CreateSlot();
		WInven->ChangeGold(Gold);
		bOnInventory = true;

		if (EquipWeaponId != -1)
			WInven->ChangeWeapon(EquipWeaponId, GInstance);

		if (EquipArmorId != -1)
			WInven->ChangeWeapon(EquipArmorId, GInstance);

		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		//	// Add to Viewport 반대 = RemoveFromViewport
		//
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
	AnimInst->PlaySkillMontage();
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
	WeaponList[WeaponIndex] = GInstance->GetWeaponData(Id);
	if (bOnInventory)
	{
		auto MyInven = Cast<UWidget_Inventory>(Inven);
		if(MyInven->GetInvenType() == 0)
			MyInven->Slots[WeaponIndex]->SetWeaponItem();
	}
	FindNextWeaponIndex();
}

void AMyPlayer::AddItemArmor(int Id)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	ArmorList[ArmorIndex] = GInstance->GetArmorData(Id);
	if (bOnInventory)
	{
		auto MyInven = Cast<UWidget_Inventory>(Inven);
		if (MyInven->GetInvenType() == 1)
			MyInven->Slots[ArmorIndex]->SetArmorItem();
	}
	FindNextArmorIndex();
}

void AMyPlayer::AddItemUse(int Id)
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	UseItemList[UseItemIndex] = GInstance->GetUseData(Id);
	if (bOnInventory)
	{
		auto MyInven = Cast<UWidget_Inventory>(Inven);
		if (MyInven->GetInvenType() == 2)
			MyInven->Slots[UseItemIndex]->SetUseItem();
	}
	FindNextUseIndex();
}

void AMyPlayer::FindNextWeaponIndex()
{
	// if 인벤토리가 가득 찼을 때 처리 TODO

	for (int i = 0; i < 24; i++)
	{
		if (WeaponList[i] == nullptr)
		{
			WeaponIndex = i;
			return;
		}
	}
}

void AMyPlayer::FindNextArmorIndex()
{
	for (int i = 0; i < 24; i++)
	{
		if (ArmorList[i] == nullptr)
		{
		ArmorIndex = i;
		return;
		}
	}
}

void AMyPlayer::FindNextUseIndex()
{
	for (int i = 0; i < 24; i++)
	{
		if (UseItemList[i] == nullptr)
		{
			UseItemIndex = i;
			return;
		}
	}
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


