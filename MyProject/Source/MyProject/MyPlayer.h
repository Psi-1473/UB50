// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Npc.h"
#include "Define.h"
#include "Portal.h"
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

public:

	void Attack();
	void SetCombo(bool Val) { bCombo = Val; }
	void EndAttack();
	void AttackCheck();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	

	void ActivateSkill(int SkillNum);
	void Fire();
	void SkillRAttackCheck();
	void SkillEAttackCheck();

	TArray<FHitResult> BoxHitResults(float X, float Y, float Z, bool IsPlayerCenter);
private:
	void StartCoolDown(int Type);
	void CoolDownQ();
	void CoolDownR();
	void CoolDownE();

	void OnDamaged();

public:
	void OpenUI(int UIType);
	void CloseUI(int UIType);
	void EquipWeapon(int Id, int Idx);
	void EquipArmor(int Id, int Idx);
	void UseItem(int Id, int Idx);

	void AddItem(int id, int ItemType);
	void AddItemWeapon(int Id);
	void AddItemArmor(int Id);
	void AddItemUse(int Id);

	int FindNextInvenIndex(int ItemType);
	bool DraggingSwap(int from, int to);

	void ChangeGold(int Value);
	void StopMoving();

	

public:
	void SetNpc(ANpc* NewNpc = nullptr){ CanInteractNpc = NewNpc; }
	void SetGold(int Value) { Gold = Value; }
	void SetOnInventory(bool Value) { bOnInventory = Value; }
	void SetInteracting(bool Value) { bInteract = Value; }
	void SetState(STATE Value) { PlayerState = Value; }
	void SetPortal(APortal* PortalValue) { Portal = PortalValue; }

	int GetGold() { return Gold; }
	STATE GetState() { return PlayerState; }
	class ANpc* GetInteractNpc() { return CanInteractNpc; }
	class APortal* GetPortal() { return Portal; }

	bool GetOnInventory() { return bOnInventory; }
	bool GetInteracting() { return bInteract; }
	
	//Temp

	int GetEquipWeaponId() { return EquipWeaponId; }
	int GetEquipArmorId() { return EquipArmorId; }

private:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnAttackMontageStarted(UAnimMontage* Montage, bool bInterrupted);

public:

	// 어택 스킬 데미지 관련 불리언 다 지우고 빌드
	UPROPERTY(EditAnywhere)
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere)
	float Horizontal;

	UPROPERTY(EditAnywhere)
	float Vertical;

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

	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

private:
	TSubclassOf<UUserWidget> Inventory;
	UUserWidget* Inven;

	TSubclassOf<UUserWidget> Conversation;
	UUserWidget* Conv;
	// 임시

private:
	TArray<int32> SkillCoolTimes;


public:
	TArray<FWeaponData*> WeaponList;
	int32 WeaponIndex = 0;
	TArray<FArmorData*> ArmorList;
	int32 ArmorIndex = 0;
	TArray<FUseItemData*> UseItemList;
	int32 UseItemIndex = 0;

	int Gold = 0;

	UPROPERTY()
	FTimerHandle QTimerHandle;
	UPROPERTY()
	FTimerHandle RTimerHandle;
	UPROPERTY()
	FTimerHandle ETimerHandle;
private:
	UPROPERTY()
	ANpc* CanInteractNpc;

	APortal* Portal;

	STATE PlayerState;

	int EquipWeaponId = -1;
	int EquipArmorId = -1;

};

