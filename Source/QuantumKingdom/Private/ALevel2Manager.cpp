// Fill out your copyright notice in the Description page of Project Settings.


#include "ALevel2Manager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AALevel2Manager::AALevel2Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DeadEnemies = 0;
	TotalEnemies = 0;
}

// Called when the game starts or when spawned
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


