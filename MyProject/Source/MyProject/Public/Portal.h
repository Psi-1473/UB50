// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class MYPROJECT_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:
	void MoveToOtherLevel();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName TransferLevelName;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* TransferVolume;

	UPROPERTY()
	class AMyPlayer* PlayerInfo;
};
