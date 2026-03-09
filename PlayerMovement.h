// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"       
#include "Components/StaticMeshComponent.h"
#include "SnakeSegment.h"
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "SnakeSegment.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "PlayerMovement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);

UCLASS()

class SNAKEGAME_API APlayerMovement : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerMovement();

	
	UFUNCTION()
	void HandelFoodPickUp(AActor* FoodActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void StartNextStep();

	FVector CurrentDirection;
	FVector TargetLocation;
	
	bool bIsMoving;

	UPROPERTY(EditAnywhere, Category = "Snake")
	float StepSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "Snake")
	float MoveSpeed = 400.f; // Units per second

	

	TArray<FVector> SegmentPositions;
	UPROPERTY(EditAnywhere, Category = "Snake")
	TSubclassOf<class ASnakeSegment> SegmentClass;

	UPROPERTY()
	TArray<ASnakeSegment*> SnakeSegments;


	int FrameCounter = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	UStaticMeshComponent* VisualMesh;

	UFUNCTION()
	void OnPlayerHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void AddBodySegment(FVector SegmentStartPosition);

	
	UFUNCTION()
	void SetBodySegmentPosition(TArray<FVector>& ArrayA, FVector Position);

	int32 MoveStepCounter = 0;
	int32 SegmentInsertCount = 0;
	const int32 StepsPerInsert = 15;
	const int32 MaxSegments = 10;
	bool bInserting = true;

	float SmoothSpeed = 6.0f;

	int SegmentCount;

	int SegmentsOnSnake;

	
	
public:	
	// Called every frame
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDeath OnPlayerDeath;
	
	// Called to bind functionality to input
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSegmentsUpdated, int32, SegmentsOnSnake);

	UPROPERTY(BlueprintAssignable)
	FOnSegmentsUpdated OnSegmentsUpdated;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* DeathEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerIndex = 0;
	
   UFUNCTION(BlueprintCallable)
    void SetDirectionExternal(const FVector& NewDir);
};
