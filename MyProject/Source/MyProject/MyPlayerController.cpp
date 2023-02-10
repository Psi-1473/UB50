// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"

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
}

void AMyPlayerController::MoveForward(float Value)
{
	if (MyPlayer->bDamaged)
		return;

	if (MyPlayer->IsAttacking)
		return;

	MyPlayer->Vertical = Value;
	GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), Value);
	
	
}
void AMyPlayerController::MoveRight(float Value)
{
	if (MyPlayer->bDamaged)
		return;

	if (MyPlayer->IsAttacking)
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
}
void AMyPlayerController::OffSprint()
{
	MyPlayer->bIsSprint = false;
}

void AMyPlayerController::ClickAttack()
{
	if (MyPlayer->bDamaged)
		return;

	if (MyPlayer->IsAttacking == false)
	{
		MyPlayer->Attack();
	}
	else
	{
		MyPlayer->bCombo = true;
	}
}

