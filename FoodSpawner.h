// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodSpawner.generated.h"

class AFoodActor;

UCLASS()
class SNAKEGAME_API AFoodSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AFoodSpawner();

protected:
	virtual void BeginPlay() override;

	FTimerHandle SpawnTimerHandle;

	void SpawnFood();

public:	

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AFoodActor> FoodClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnDelay = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector2D SpawnAreaMin = FVector2D(-500.f, -500.f);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector2D SpawnAreaMax = FVector2D(500.f, 500.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* EatAppleSound;
};
