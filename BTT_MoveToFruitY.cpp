// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToFruitY.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AISnakePawn.h"
#include "Kismet/GameplayStatics.h"
#include "FoodActor.h"

UBTT_MoveToFruitY::UBTT_MoveToFruitY()
{
	bNotifyTick = true;
	NodeName = "Move To Fruit Y";
}


EBTNodeResult::Type UBTT_MoveToFruitY::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	AAISnakePawn* SnakePawn = Cast<AAISnakePawn>(AIPawn);
	if (!SnakePawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	FVector FruitLocation = BlackboardComp->GetValueAsVector("ClosestFruit");
	FVector SnakeLocation = SnakePawn->GetActorLocation();

	const float Tolerance = 10.0f;
	if (FMath::Abs(SnakeLocation.Y - FruitLocation.Y) < Tolerance)
	{
		return EBTNodeResult::Succeeded;
	}

	FVector NewDirection = FVector::ZeroVector;

	if (FruitLocation.Y > SnakeLocation.Y)
		NewDirection = FVector(0, 1, 0); // Move up
	else if (FruitLocation.Y < SnakeLocation.Y)
		NewDirection = FVector(0, -1, 0); // Move down

	SnakePawn->CurrentDirection = NewDirection;

	return EBTNodeResult::InProgress;
}

void UBTT_MoveToFruitY::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!AIPawn) return;

	AAISnakePawn* SnakePawn = Cast<AAISnakePawn>(AIPawn);
	if (!SnakePawn) return;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	FVector FruitLocation = BlackboardComp->GetValueAsVector("ClosestFruit");
	FVector SnakeLocation = SnakePawn->GetActorLocation();

	const float Tolerance = 10.0f;

	if (FMath::Abs(SnakeLocation.Y - FruitLocation.Y) < Tolerance)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (FruitLocation.Y > SnakeLocation.Y)
		SnakePawn->CurrentDirection = FVector(0, 1, 0);
	else
		SnakePawn->CurrentDirection = FVector(0, -1, 0);
}