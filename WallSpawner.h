// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallSpawner.generated.h"

UCLASS()
class SNAKEGAME_API AWallSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSet<FVector> UsedPositions;

	FVector GetRandomGridPosition();

public:	

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> WallClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 NumberOfWalls = 10;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector2D SpawnAreaMin = FVector2D(-500, -500);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector2D SpawnAreaMax = FVector2D(500, 500);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float GridSize = 100.0f;  // Ensures clean spacing

	// Player start exclusion radius
	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector PlayerStartLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float ExclusionRadius = 300.0f; // Adjust as needed
	
};
