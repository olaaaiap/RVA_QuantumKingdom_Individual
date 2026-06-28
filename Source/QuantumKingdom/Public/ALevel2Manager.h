// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALevel2Manager.generated.h"

UCLASS()
class QUANTUMKINGDOM_API AALevel2Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AALevel2Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	int32 DeadEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	int32 TotalEnemies;

public:	
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void AddEnemyDead();
};
