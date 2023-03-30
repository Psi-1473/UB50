// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnMonster.h"
#include "MonsterSpawner.h"
#include "Components/WidgetComponent.h"
#include "Widget_Hp.h"
#include "EnemyAnimInstance.h"

ASpawnMonster::ASpawnMonster()
{
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));

	HpBar->SetupAttachment(GetMesh());
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HPBar.WBP_HPBar_C'"));

	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2d(200.f, 50.f));
		HpBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	}
}



void ASpawnMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	HpBar->InitWidget();
	
	auto Bar = Cast<UWidget_Hp>(HpBar->GetUserWidgetObject());
	Bar->BindWidget_Enemy(Stat);
}

void ASpawnMonster::SetSpawner(AMonsterSpawner* Spawner)
{
	MySpawner = Spawner;
	MySpawner->UpCount();
}
void ASpawnMonster::OnDamaged()
{
	Super::OnDamaged();
	SetState(DAMAGED);
	AnimInst->PlayDamagedMontage();
}

void ASpawnMonster::Attack(AMyPlayer* Target)
{
	if (MonsterState == DAMAGED || MonsterState == DIED)
		return;
	Super::Attack(Target);
	AnimInst->PlayAttackMontage();
}

void ASpawnMonster::Die(AMyPlayer* Player)
{
	Super::Die(Player);
	AnimInst->PlayDeathMontage();
	MySpawner->DownCount();
}

void ASpawnMonster::OnStun(float Tick)
{
	Super::OnStun(Tick);
	AnimInst->StopAllMontages(0.5f);
}
