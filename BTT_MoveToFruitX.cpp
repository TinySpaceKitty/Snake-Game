// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToFruitX.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AISnakePawn.h"
#include "Kismet/GameplayStatics.h"
#include "FoodActor.h"

UBTT_MoveToFruitX::UBTT_MoveToFruitX()
{
	bNotifyTick = true;
	NodeName = "Move To Fruit X";
}

EBTNodeResult::Type UBTT_MoveToFruitX::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	if (FMath::Abs(SnakeLocation.X - FruitLocation.X) < Tolerance)
	{
		return EBTNodeResult::Succeeded;
	}

	FVector NewDirection = FVector::ZeroVector;

	if (FruitLocation.X > SnakeLocation.X)
		NewDirection = FVector(1, 0, 0); // Move up
	else if (FruitLocation.X < SnakeLocation.X)
		NewDirection = FVector(-1, 0, 0); // Move down

	SnakePawn->CurrentDirection = NewDirection;

	return EBTNodeResult::InProgress;
}

void UBTT_MoveToFruitX::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	if (FMath::Abs(SnakeLocation.X - FruitLocation.X) < Tolerance)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (FruitLocation.X > SnakeLocation.X)
		SnakePawn->CurrentDirection = FVector(1, 0, 0);
	else
		SnakePawn->CurrentDirection = FVector(-1, 0, 0);
}
