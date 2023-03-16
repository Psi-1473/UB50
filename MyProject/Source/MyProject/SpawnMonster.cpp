// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnMonster.h"
#include "MonsterSpawner.h"
#include "Components/WidgetComponent.h"
#include "Widget_Hp.h"

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


	HpBar->InitWidget();
	
	auto Bar = Cast<UWidget_Hp>(HpBar->GetUserWidgetObject());
	Bar->BindWidget_Enemy(Stat);
}

void ASpawnMonster::SetSpawner(AMonsterSpawner* Spawner)
{
	MySpawner = Spawner;
	MySpawner->UpCount();
}

void ASpawnMonster::Die(AMyPlayer* Player)
{
	Super::Die(Player);
	MySpawner->DownCount();
}
