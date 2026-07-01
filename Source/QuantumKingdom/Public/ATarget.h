// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATarget.generated.h"

UCLASS()
class QUANTUMKINGDOM_API AATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	AATarget();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* Box;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
    bool IsMobile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
    FVector Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
    float Amplitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
    float Speed;

private:
    FVector Origin;
    FVector DirecNorm;
    float TimeCount;

    UFUNCTION()
    void HandleOverlap(AActor* OverlappedActor, AActor* OtherActor);

    bool CanCollide(AActor* OtherActor);
    void OnCollide();
};
