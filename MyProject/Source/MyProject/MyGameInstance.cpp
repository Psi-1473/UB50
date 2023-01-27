// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Data(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));;
	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyData(TEXT("DataTable'/Game/Data/EnemyStatTable.EnemyStatTable'"));;
		
	MyStats = Data.Object;
	EnemyStats = EnemyData.Object;

}

void UMyGameInstance::Init()
{
	UGameInstance::Init();
	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %d"), GetStatData(1)->Attack);

	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %s"), *GetEnemyData(TEXT("Kwang"))->Name);
}


FMyCharacterData* UMyGameInstance::GetStatData(int32 Level)
{
	return MyStats->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}

FEnemyData* UMyGameInstance::GetEnemyData(FString Kwang)
{
	return EnemyStats->FindRow<FEnemyData>(*Kwang, TEXT(""));
}
