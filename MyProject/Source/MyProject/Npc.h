// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interactable.h"
#include "Manager_Script.h"
#include "Npc.generated.h"


struct FScript
{
public:
	TArray<FNpcScriptData> Scripts;
};

UCLASS()
class MYPROJECT_API ANpc : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANpc();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Interact(class AMyPlayer* Player) override;
	virtual void EndInteract() override {};


	void RemovePossibleQuest(int QuestId) { PossibleQuests.Remove(QuestId); }
	void RemoveCanClearQuest(int QuestId) { CanClearQuests.Remove(QuestId); }
	
	void AddToPossibleQuest(int QuestId) { PossibleQuests.Add(QuestId); }
	void AddToCanClearQuest(int QuestId);

	void AddScriptData(FNpcScriptData Data);

	int GetTotalItemNum();
	int GetItem(int Id);

	bool GetIsShop() { return IsShopNpc; }
	FString GetName() { return NpcName; }
	FString GetLine() { return BaseScript; }
	int GetNpcId() { return NpcId; }
	TArray<int> GetPossibleQuests() { return PossibleQuests; }
	TArray<int> GetCanClearQuests() { return CanClearQuests; }
	TArray<FNpcScriptData> GetScriptData(int QuestId) { return ScriptData[QuestId].Scripts; }
	
	UPROPERTY(EditAnywhere)
	int ItemType;
	void SetQuestMark();
private:
	void InitNpcId();
	void InitQuestInfo();
private:
	int NpcId;

	UPROPERTY()
	FString NpcName;
	
	UPROPERTY(EditAnywhere)
	TArray<int> WeaponId;

	UPROPERTY(EditAnywhere)
	TArray<int> ArmorId;

	UPROPERTY(EditAnywhere)
	TArray<int> UseId;
	// Todo
	// 가진 퀘스트 목록
	// 상점 아이템 리스트
	// 스크립트?
	UPROPERTY(EditAnywhere)
	bool IsShopNpc;

	FString BaseScript;

	TArray<int> PossibleQuests;
	TArray<int> CanClearQuests;

	TMap<int, FScript> ScriptData;

private:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InteractionKey;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* QuestMark;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* QuestCompMark;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* InteractBox;
};
