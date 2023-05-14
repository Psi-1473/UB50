// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Interactable.h"
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
	void Recovery(int NumberToIncrease);
	void Respawn();

	void PlaySound(int Number);

public:
	void OpenUI(UIType MyUIType);
	void CloseUI(UIType MyUIType);

	void StopMoving();
	void CloseCursorInGame();
	void PlayUiSound();
	void PlayQuestClearSound();
	void PlayEquipSound();

public:
	void SetNpc(ANpc* NewNpc = nullptr){ CanInteractNpc = NewNpc; }

	void SetOnQuest(bool Value) { bOnQuest = Value; }
	void SetInteracting(bool Value) { bInteract = Value; }
	void SetState(STATE Value) { PlayerState = Value; }
	void SetPortal(APortal* PortalValue) { Portal = PortalValue; }
	void SetInteractObj(IInteractable* InterValue) { InteractObj = InterValue; }

	class UMyAnimInstance* GetAnimInst() { return AnimInst; }
	STATE GetState() { return PlayerState; }
	class ANpc* GetInteractNpc() { return CanInteractNpc; }
	class APortal* GetPortal() { return Portal; }
	class IInteractable* GetInteractObj() { return InteractObj; }

	

	bool GetOnQuest() { return bOnQuest; }
	bool GetInteracting() { return bInteract; }
	
	//Temp

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
	bool bOnUI = false;

	UPROPERTY()
	int SkillRCombo = 0;

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
	class UPlayerAudioComponent* AudioPlayer;

	UPROPERTY()
	bool bOnQuest = false; //임시 나중에 UI 매니저에서 관리
	UPROPERTY()
	bool bInteract = false;

	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

private:
	TArray<int32> SkillCoolTimes;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* SkillREmitter;

public:

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

	IInteractable* InteractObj;

	STATE PlayerState;

	FVector SpawnSpot;

	class USoundCue* UICue;
	class USoundWave* EquipCue;
	class USoundCue* QuestClearCue;

};

