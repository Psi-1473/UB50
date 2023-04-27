// Fill out your copyright notice in the Description page of Project Settings.


#include "Npc.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "MyPlayer.h"
#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Manager_Quest.h"



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

	InitNpcId();
}

// Called when the game starts or when spawned
void ANpc::BeginPlay()
{
	Super::BeginPlay();

	InitQuestInfo();

	UMyGameInstance* GInstance = Cast<UMyGameInstance>(GetGameInstance());
	GInstance->QuestManager->AddNpc(NpcId, this);
	GInstance->ScriptManager->LoadScriptData(GInstance, NpcId);
	UE_LOG(LogTemp, Warning, TEXT("Npc : %s"), *BaseScript);

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
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto Player = Cast<AMyPlayer>(OtherActor);
		if (Player)
		{
			InteractionKey->SetVisibility(true);
			Player->SetInteractObj(this);
		}
	}
}

void ANpc::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto Player = Cast<AMyPlayer>(OtherActor);
		if (Player)
		{
			InteractionKey->SetVisibility(false);
			if (Player->GetInteractObj() == this)
				Player->SetInteractObj(nullptr);

			Player->SetNpc(nullptr);
		}
	}
}

void ANpc::Interact(AMyPlayer* Player)
{
	Player->SetNpc(this);
	if (Player->GetInteracting())
	{
		Player->SetInteracting(false);
		Player->CloseUI(UIType::ALL);
	}
	else
	{
		Player->SetInteracting(true);
		Player->OpenUI(UIType::CONVERSATION);
	}
}

void ANpc::AddToCanClearQuest(int QuestId)
{
	CanClearQuests.Add(QuestId);
	// 만약에 처음 추가된거면 Npc 머리 위에 마크 띄우기
}

void ANpc::AddScriptData(FNpcScriptData Data)
{
	if (Data.QuestId == -1)
	{
		BaseScript = Data.Line;
		return;
	}
	
	if (!ScriptData.Contains(Data.QuestId))
	{
		FScript NewScript;
		ScriptData.Add(Data.QuestId, NewScript);
		UE_LOG(LogTemp, Warning, TEXT("New Npc Scripts %d, %d"), NpcId, Data.QuestId);
	}

	ScriptData[Data.QuestId].Scripts.Add(Data);
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

void ANpc::InitNpcId()
{
	FString MyName = GetActorNameOrLabel();
	FString BaseName = TEXT("BP_Npc");
	int FullNameLen = MyName.Len();
	int Len = BaseName.Len();
	int Count = FullNameLen = Len;
	
	FString IdName = MyName.Mid(Len, Count);
	NpcId = FCString::Atoi(*IdName);
	UE_LOG(LogTemp, Warning, TEXT("Npc Id Registered! : %d"), NpcId);
}

void ANpc::InitQuestInfo()
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UseGInstance
	if (GInstance->QuestManager->GetQuestsByNpcId(NpcId).IsEmpty())
		return;

	TArray<FQuest> Quests = GInstance->QuestManager->GetQuestsByNpcId(NpcId);

	for (int i = 0; i < Quests.Num(); i++)
	{
		if(!Quests[i].Locked)
			PossibleQuests.Add(Quests[i].Id);
	}
}

