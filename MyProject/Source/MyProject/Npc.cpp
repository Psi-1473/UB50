// Fill out your copyright notice in the Description page of Project Settings.


#include "Npc.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "MyPlayer.h"

// Sets default values
ANpc::ANpc()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionKey = CreateDefaultSubobject<UWidgetComponent>(TEXT("INTERACTIONKEY"));

	InteractionKey->SetupAttachment(GetMesh());
	InteractionKey->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_Interaction.WBP_Interaction_C'"));

	if (UW.Succeeded())
	{
		InteractionKey->SetWidgetClass(UW.Class);
		InteractionKey->SetDrawSize(FVector2d(50.f, 50.f));
		InteractionKey->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	}

	InteractionKey->SetVisibility(false);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(GetMesh());

	InteractBox->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	InteractBox->SetRelativeScale3D(FVector(4.0f, 4.5f, 3.0f));
	InteractBox->OnComponentBeginOverlap.AddDynamic(this, &ANpc::OnOverlapBegin);
	InteractBox->OnComponentEndOverlap.AddDynamic(this, &ANpc::OnOverlapEnd);

	InteractBox->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel2);
	UE_LOG(LogTemp, Warning, TEXT("Npc On"));

	
}

// Called when the game starts or when spawned
void ANpc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANpc::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Someone Entered"));
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto Player = Cast<AMyPlayer>(OtherActor);
		if (Player)
		{
			InteractionKey->SetVisibility(true);
			Player->SetNpc(this);
		}
	}
}

void ANpc::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Someone Out"));
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto Player = Cast<AMyPlayer>(OtherActor);
		if (Player)
		{
			InteractionKey->SetVisibility(false);
			Player->SetNpc();
		}
	}
}

int ANpc::GetTotalItemNum()
{
	if(ItemType == 0)
		return WeaponId.Num();
	else if(ItemType == 1)
		return ArmorId.Num();
	else
		return UseId.Num();
}

int ANpc::GetItem(int Id)
{
	if (ItemType == 0)
		return WeaponId[Id];
	else if (ItemType == 1)
		return ArmorId[Id];
	else
		return UseId[Id];
}

