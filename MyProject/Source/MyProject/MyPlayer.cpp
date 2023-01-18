// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayer.h"
#include "MyAnimInstance.h"

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



}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	AnimInst = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInst->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnAttackMontageEnded);
	
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyPlayer::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPlayer::Pitch);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyPlayer::OnSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyPlayer::OffSprint);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyPlayer::ClickAttack);

}

void AMyPlayer::MoveForward(float Value)
{
	if (IsAttacking)
		return;

	Vertical = Value;
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyPlayer::MoveRight(float Value)
{
	if (IsAttacking)
		return;

	Horizontal = Value;
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyPlayer::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMyPlayer::Pitch(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyPlayer::OnSprint()
{
	bIsSprint = true;
}

void AMyPlayer::OffSprint()
{
	bIsSprint = false;
}

void AMyPlayer::ClickAttack()
{
	if (IsAttacking == false)
	{
		Attack();
	}
	else
	{
		bCombo = true;
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
	//if (!AnimInst->Montage_IsPlaying(AnimInst->AttackMontage))
	//{
	//	AnimInst->PlayAttackMontage();
	//}
	//else if(AnimInst->Montage_IsPlaying(AnimInst->AttackMontage))
	//{
	//	
	//	
	//}
	
	
}

void AMyPlayer::EndAttack()
{
	IsAttacking = false;
	bCombo = false;
	AttackIndex = 0;
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


