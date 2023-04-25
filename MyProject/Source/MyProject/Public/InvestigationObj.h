// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InvestigationObj.generated.h"

UCLASS()
class MYPROJECT_API AInvestigationObj : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AInvestigationObj();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class AMyPlayer* Player) override;
	virtual void EndInteract() override;
	void SetInteractable(bool Value) { bInteractable = Value; }

private:
	void ExtractId();
	
public:
	int GetId() { return Id; }

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* TransferVolume;
private:
	UPROPERTY(EditAnywhere)
	int InvestigationTime;

	int Id;

	bool bInteractable;
};
