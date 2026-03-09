// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FoodActor.h"
#include "BTS_UpdatePlayerValues.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API UBTS_UpdatePlayerValues : public UBTService
{
	GENERATED_BODY()

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	

	void GetAllFruitActors(UWorld* World, TArray<AFoodActor*>& OutFruits);
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelfActorKey;

	UPROPERTY(EditAnywhere, Category = BlackBoard)
	FBlackboardKeySelector PlayerPosKey;

	UPROPERTY(EditAnywhere, Category = BlackBoard)
	FBlackboardKeySelector CurrentDirKey;

	UPROPERTY(EditAnywhere, Category = BlackBoard)
	FBlackboardKeySelector ClosestFruitKey;

	UPROPERTY(EditAnywhere, Category = BlackBoard)
	FBlackboardKeySelector SegmentsKey;
};
