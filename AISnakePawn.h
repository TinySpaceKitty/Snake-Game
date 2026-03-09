// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "SnakeSegment.h"
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "AISnakePawn.generated.h"

UCLASS()
class SNAKEGAME_API AAISnakePawn : public APawn 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BTAsset;
	
	AAISnakePawn();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	UPROPERTY()
	FVector PendingDirection = FVector::ZeroVector;

	UPROPERTY()
	bool bHasPendingDirection = false;

	void SetDirectionTowardsX(float FruitX);
	void SetDirectionTowardsY(float FruitY);
	
	// Movement logic
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	FVector CurrentDirection;
	FVector TargetLocation;
	bool bIsMoving;
	float MoveSpeed = 300.f;
	float StepSize = 100.f;


	bool bShouldReturnToQueuedDirection = false;
	
	TArray<FVector> SegmentPositions;
	TArray<ASnakeSegment*> SnakeSegments;
	int32 SegmentInsertCount = 10;
	int32 SegmentCount = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeSegment> SegmentClass;

	FTimerHandle MovementTimer;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* DeathEffect;
	
	// AI Logic
	void StartNextStep();
	void AddBodySegment(FVector SpawnLocation);
	void SetBodySegmentPosition(TArray<FVector>& ArrayA, FVector Position);

	// Overlap handler
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetDirectionSafe(FVector DesiredDirection);
	
	// Fruit pickup trigger
	void HandleFruitPickup();
	bool IsWallAhead(const FVector& Direction);
};
