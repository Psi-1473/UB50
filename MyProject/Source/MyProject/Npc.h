// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Npc.generated.h"

UCLASS()
class MYPROJECT_API ANpc : public ACharacter
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



	int GetTotalItemNum();
	int GetItem(int Id);

	bool GetIsShop() { return IsShopNpc; }
	FString GetName() { return NpcName; }
	
	
	UPROPERTY(EditAnywhere)
	int ItemType;
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

	TArray<int> PossibleQuests;
	//TArray<int> StartedQuests;
	//TArray<int> EndedQuests;
	TArray<int> ImpossibleQuests;


private:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InteractionKey;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* InteractBox;
};
