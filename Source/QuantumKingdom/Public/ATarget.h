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
	// Sets default values for this actor's properties
	AATarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Componentes
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* Box;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* StaticMesh;

    // Variables editables desde el editor
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
