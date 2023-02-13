// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Npc.h"
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
	void SetNpc(ANpc* NewNpc = nullptr){ CanInteractNpc = NewNpc; }
	void Interact();
	void TestAddItem();
	void FindNextWeaponIndex();

private:
	void PopupInventory();

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

	UPROPERTY()
	bool bOnInventory = false; //임시 나중에 UI 매니저에서 관리
	UPROPERTY()
	bool bInteract = false;

private:
	TSubclassOf<UUserWidget> Inventory;
	UUserWidget* Inven;

	TSubclassOf<UUserWidget> Conversation;
	UUserWidget* Conv;
	// 임시

public:
	TArray<FWeaponData*> WeaponList;
	int32 WeaponIndex = 0;
	TArray<FArmorData*> ArmorList;
	int32 ArmorIndex = 0;
	TArray<FUseItemData*> UseItemList;
	int32 UseItemIndex = 0;

private:
	UPROPERTY()
	ANpc* CanInteractNpc;

};

