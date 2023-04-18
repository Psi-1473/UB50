// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Manager_Inven.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AMyPlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay Controller"));
	MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Possess(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("Yaw"), this, &AMyPlayerController::Yaw);
	InputComponent->BindAxis(TEXT("Pitch"), this, &AMyPlayerController::Pitch);

	
	InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyPlayerController::OnSprint);
	InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyPlayerController::OffSprint);
	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyPlayerController::ClickAttack);
	InputComponent->BindAction(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &AMyPlayerController::ClickRSkill);
	InputComponent->BindAction(TEXT("Skill2"), EInputEvent::IE_Pressed, this, &AMyPlayerController::ClickQSkill);
	InputComponent->BindAction(TEXT("Skill3"), EInputEvent::IE_Pressed, this, &AMyPlayerController::ClickESkill);
	InputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &AMyPlayerController::PopupInventory);
	InputComponent->BindAction(TEXT("Quest"), EInputEvent::IE_Pressed, this, &AMyPlayerController::PopupQuest);
	InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyPlayerController::Interact);
}

void AMyPlayerController::MoveForward(float Value)
{
	if (MyPlayer->GetState() == DAMAGED)
		return;

	if (MyPlayer->GetState() == ATTACK)
		return;

	if (MyPlayer->GetState() == SKILL)
		return;

	MyPlayer->Vertical = Value;
	GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), Value);
	
}
void AMyPlayerController::MoveRight(float Value)
{
	if (MyPlayer->GetState() == DAMAGED)
		return;

	if (MyPlayer->GetState() == ATTACK)
		return;

	if (MyPlayer->GetState() == SKILL)
		return;

	MyPlayer->Horizontal = Value;
	GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), Value);
}

void AMyPlayerController::Yaw(float Value)
{
	AddYawInput(Value);
}
void AMyPlayerController::Pitch(float Value)
{
	AddPitchInput(Value);
}

void AMyPlayerController::OnSprint()
{
	MyPlayer->bIsSprint = true;
	MyPlayer->GetCharacterMovement()->MaxWalkSpeed = 800.f;


}
void AMyPlayerController::OffSprint()
{
	MyPlayer->bIsSprint = false;
	MyPlayer->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMyPlayerController::ClickAttack()
{
	if (MyPlayer->GetState() == SKILL)
		return;

	if (MyPlayer->GetState() != ATTACK)//�޺��κ�
	{
		MyPlayer->Attack();
	}
	else
	{
		MyPlayer->bCombo = true;
	}
}

void AMyPlayerController::ClickRSkill()
{
	if (MyPlayer->GetState() == ATTACK)
		return;

	if (MyPlayer->GetState() == SKILL)
		return;

	MyPlayer->ActivateSkill(PLAYERSKILL_R);
}

void AMyPlayerController::ClickQSkill()
{
	if (MyPlayer->GetState() == ATTACK)
		return;

	if (MyPlayer->GetState() == SKILL)
		return;

	MyPlayer->ActivateSkill(PLAYERSKILL_Q);
}

void AMyPlayerController::ClickESkill()
{
	if (MyPlayer->GetState() == ATTACK)
		return;

	if (MyPlayer->GetState() == SKILL)
		return;

	MyPlayer->ActivateSkill(PLAYERSKILL_E);
}

void AMyPlayerController::Interact()
{
	if (MyPlayer->GetPortal() != nullptr)
	{
		MyPlayer->GetPortal()->MoveToOtherLevel();
		return;
	}

	if (MyPlayer->GetInteractNpc() == nullptr)
		return;

	if (MyPlayer->GetInteracting())
	{
		MyPlayer->SetInteracting(false);
		MyPlayer->CloseUI(UIType::CONVERSATION);
		MyPlayer->CloseUI(UIType::SHOP);
		MyPlayer->CloseUI(UIType::QUEST);
	}
	else
	{
		MyPlayer->SetInteracting(true);
		MyPlayer->OpenUI(UIType::CONVERSATION);
	}
}

void AMyPlayerController::PopupInventory()
{
	if(MyPlayer == nullptr)
		MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	UseGInstance

	if (GInstance->InvenManager->GetOnInventory())
	{
		MyPlayer->CloseUI(UIType::INVENTORY);
		GInstance->InvenManager->SetOnInventory(false);
	}
	else
	{
		MyPlayer->OpenUI(UIType::INVENTORY);
		GInstance->InvenManager->SetOnInventory(true);
	}
}

void AMyPlayerController::PopupQuest()
{
	if (MyPlayer == nullptr)
		MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (MyPlayer->GetOnQuest())
	{
		MyPlayer->CloseUI(UIType::PLAYERQUEST);
		MyPlayer->SetOnQuest(false);
	}
	else
	{
		MyPlayer->OpenUI(UIType::PLAYERQUEST);
		MyPlayer->SetOnQuest(true);
	}
	
}

