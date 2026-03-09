// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpawner.h"

#include "PlayerMovement.h"

// Sets default values
AWallSpawner::AWallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWallSpawner::BeginPlay()
{
	Super::BeginPlay();

	APlayerMovement* Player = Cast<APlayerMovement>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player)
	{
		PlayerStartLocation = Player->GetActorLocation();
	}
	
	if (!WallClass) return;

	for (int32 i = 0; i < NumberOfWalls; i++)
	{
		FVector Position = GetRandomGridPosition();

		GetWorld()->SpawnActor<AActor>(
			WallClass,
			Position,
			FRotator::ZeroRotator
		);
	}
}

FVector AWallSpawner::GetRandomGridPosition()
{
	const int MaxAttempts = 50;
	int Attempts = 0;

	while (Attempts++ < MaxAttempts)
	{
		float RandX = FMath::RandRange(SpawnAreaMin.X, SpawnAreaMax.X);
		float RandY = FMath::RandRange(SpawnAreaMin.Y, SpawnAreaMax.Y);

		// Snap to grid
		RandX = FMath::GridSnap(RandX, GridSize);
		RandY = FMath::GridSnap(RandY, GridSize);

		FVector Position(RandX, RandY, 0);

		// Avoid already used and avoid player start area
		if (!UsedPositions.Contains(Position) &&
			FVector::Dist2D(Position, PlayerStartLocation) > ExclusionRadius)
		{
			UsedPositions.Add(Position);
			return Position;
		}
	}

	// Fallback
	return FVector::ZeroVector;
}

