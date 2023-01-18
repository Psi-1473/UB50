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

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);

	void OnSprint();
	void OffSprint();
	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(EditAnywhere)
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere)
	float Horizontal;

	UPROPERTY(EditAnywhere)
	float Vertical;

private:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	bool IsAttacking = false;

	UPROPERTY()
	class UMyAnimInstance* AnimInst;

	UPROPERTY()
	int32 AttackIndex = 0;

};
