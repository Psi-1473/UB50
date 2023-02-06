// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayer.generated.h"

UCLASS()
class MYPROJECT_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMyPlayer();

protected:
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;
public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Attack();
	void SetCombo(bool Val) { bCombo = Val; }
	void EndAttack();
	void AttackCheck();
	void OnDamaged();

public:
	void SetDamaged(bool Value) { bDamaged = Value; }
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);

	void OnSprint();
	void OffSprint();
	void ClickAttack();
	
	

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnAttackMontageStarted(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(EditAnywhere)
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere)
	float Horizontal;

	UPROPERTY(EditAnywhere)
	float Vertical;
	UPROPERTY(EditAnywhere)
	bool IsAttacking = false;

	UPROPERTY(EditAnywhere)
	bool bDamaged = false;

	UPROPERTY(EditAnywhere)
	bool bCombo = false;

	UPROPERTY(EditAnywhere)
	int32 AttackIndex = 0;

	UPROPERTY(VisibleAnywhere)
	class UPlayerStatComponent* Stat;

private:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY()
	class UMyAnimInstance* AnimInst;

	UPROPERTY()
	class AMyGameMode* GameMode;



};

