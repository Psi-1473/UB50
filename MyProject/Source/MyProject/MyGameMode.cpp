// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "MyGameMode.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMyPlayer::StaticClass();

}
