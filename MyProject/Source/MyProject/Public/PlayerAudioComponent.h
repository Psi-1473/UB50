// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "PlayerAudioComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UPlayerAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	UPlayerAudioComponent();
	void PlaySound(int SoundNumber);
	void Play2DSound(int SoundNumber);

private:

	UPROPERTY()
	TArray<class USoundCue*> PlayerCues;
};
