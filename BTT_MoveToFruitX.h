// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveToFruitX.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API UBTT_MoveToFruitX : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MoveToFruitX();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool bReachedX = false;
};
