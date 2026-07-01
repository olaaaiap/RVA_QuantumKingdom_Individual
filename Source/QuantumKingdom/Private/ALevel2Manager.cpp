// Fill out your copyright notice in the Description page of Project Settings.


#include "ALevel2Manager.h"
#include <Kismet/GameplayStatics.h>

AALevel2Manager::AALevel2Manager()
{
	PrimaryActorTick.bCanEverTick = true;

	DeadEnemies = 0;
	TotalEnemies = 0;
}

void AALevel2Manager::BeginPlay()
{
	Super::BeginPlay();
}

void AALevel2Manager::AddEnemyDead()
{
	DeadEnemies++;

	if (DeadEnemies >= TotalEnemies)
	{
		UGameplayStatics::OpenLevel(this, FName("VictoryShip"));
	}
}


