// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayer.h"
#include "MyAnimInstance.h"
#include "EnemyCharKwang.h"
#include "PlayerStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget_PlayerMain.h"
#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Widget_Inventory.h"
#include "Widget_InvenSlot.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);


	SpringArm->TargetArmLength = 500.f;
	Camera->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	

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
	static ConstructorHelpers::FClassFinder<UUserWidget> CW(TEXT("WidgetBlueprint'/Game/UI/WBP_Inventory.WBP_Inventory_C'"));
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
	PlayerInputComponent->BindAction(TEXT("TestKey"), EInputEvent::IE_Pressed, this, &AMyPlayer::TestAddItemWeapon);
	PlayerInputComponent->BindAction(TEXT("TestKey2"), EInputEvent::IE_Pressed, this, &AMyPlayer::TestAddItemArmor);
	PlayerInputComponent->BindAction(TEXT("TestKey3"), EInputEvent::IE_Pressed, this, &AMyPlayer::TestAddItemUse);

}
void AMyPlayer::PopupInventory()
{
	if (bOnInventory)
	{
		Inven->RemoveFromViewport();
		bOnInventory = false;
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		if(Inven == nullptr)
			Inven = CreateWidget(GetWorld(), Inventory);
		
		UE_LOG(LogTemp, Warning, TEXT("GameMode CurrentWidget Succeeded!"));
		Inven->AddToViewport();
		UWidget_Inventory* WInven = Cast<UWidget_Inventory>(Inven);
		WInven->CreateSlot();
		bOnInventory = true;
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		//	// Add to Viewport 반대 = RemoveFromViewport
		//
	}
}
void AMyPlayer::Attack()
{
	if (AttackIndex >= 3)
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
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
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



	if (bResult && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
		AEnemyCharKwang* Enemy = Cast<AEnemyCharKwang>(HitResult.GetActor());

		//Enemy->OnDamaged();
		FDamageEvent DamageEvent;
		Enemy->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
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
		if (Conv == nullptr)
			Conv = CreateWidget(GetWorld(), Conversation);

		UE_LOG(LogTemp, Warning, TEXT("GameMode CurrentWidget Succeeded!"));
		Conv->AddToViewport();
		bInteract = true;
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		//	// Add to Viewport 반대 = RemoveFromViewport
		//
	}

}

void AMyPlayer::TestAddItemWeapon()
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	WeaponList[WeaponIndex] = GInstance->GetWeaponData(1);
	if (bOnInventory)
	{
		auto MyInven = Cast<UWidget_Inventory>(Inven);
		if(MyInven->GetInvenType() == 0)
			MyInven->Slots[WeaponIndex]->SetWeaponItem();
	}
	FindNextWeaponIndex();
}

void AMyPlayer::TestAddItemArmor()
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	ArmorList[ArmorIndex] = GInstance->GetArmorData(1);
	if (bOnInventory)
	{
		auto MyInven = Cast<UWidget_Inventory>(Inven);
		if (MyInven->GetInvenType() == 1)
			MyInven->Slots[ArmorIndex]->SetArmorItem();
	}
	FindNextArmorIndex();
}

void AMyPlayer::TestAddItemUse()
{
	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	UseItemList[UseItemIndex] = GInstance->GetUseData(1);
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


