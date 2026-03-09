// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodSpawner.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "FoodActor.h"

// Sets default values
AFoodSpawner::AFoodSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFoodSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AFoodSpawner::SpawnFood, SpawnDelay, true);
}

void AFoodSpawner::SpawnFood()
{
	if (!FoodClass) return;

	float X = FMath::FRandRange(SpawnAreaMin.X, SpawnAreaMax.X);
	float Y = FMath::FRandRange(SpawnAreaMin.Y, SpawnAreaMax.Y);
	FVector SpawnLocation = GetActorLocation() + FVector(X, Y, 0.0f);

	GetWorld()->SpawnActor<AFoodActor>(FoodClass, SpawnLocation, FRotator::ZeroRotator);
}

