// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAudioComponent.h"
#include "../DEFINE.H"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UPlayerAudioComponent::UPlayerAudioComponent()
{
	PlayerCues.Init(nullptr, SOUNDEND);

	static ConstructorHelpers::FObjectFinder<USoundCue> AudioFoot(TEXT("SoundCue'/Game/Sound/Player/Step.Step'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> AudioQ(TEXT("SoundCue'/Game/Sound/Player/SkillQ.SkillQ'"));
	if (AudioFoot.Succeeded())
		PlayerCues[FOOT] = AudioFoot.Object;
	if (AudioQ.Succeeded())
		PlayerCues[SKILLSOUNDQ] = AudioQ.Object;
}


void UPlayerAudioComponent::PlaySound(int SoundNumber)
{
	SetSound(PlayerCues[SoundNumber]);
	Play();
}

void UPlayerAudioComponent::Play2DSound(int SoundNumber)
{
	
}

