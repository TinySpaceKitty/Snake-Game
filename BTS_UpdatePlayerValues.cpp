// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdatePlayerValues.h"
#include "AISnakePawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FoodActor.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTS_UpdatePlayerValues::TickNode(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	APawn* SelfActor = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfActorKey.SelectedKeyName));
	
	if (SelfActor)
	{
		FVector Pos = SelfActor->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(PlayerPosKey.SelectedKeyName, Pos);

		AAISnakePawn* SnakePawn = Cast<AAISnakePawn>(SelfActor);
		if (SnakePawn)
		{
			FVector CurrentDir = SnakePawn->CurrentDirection;
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(CurrentDirKey.SelectedKeyName, CurrentDir);
		}
	}

	TArray<AFoodActor*> Fruits;
	GetAllFruitActors(GetWorld(), Fruits);

	if (Fruits.Num() > 0)
	{
		// Find nearest fruit
		APawn* SelfPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (SelfPawn)
		{
			FVector SelfLocation = SelfPawn->GetActorLocation();
			AFoodActor* Closest = nullptr;
			float ClosestDistanceSq = TNumericLimits<float>::Max();

			for (AFoodActor* Fruit : Fruits)
			{
				float DistSq = FVector::DistSquared(SelfLocation, Fruit->GetActorLocation());
				if (DistSq < ClosestDistanceSq)
				{
					ClosestDistanceSq = DistSq;
					Closest = Fruit;
				}
			}

			if (Closest)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(ClosestFruitKey.SelectedKeyName, Closest->GetActorLocation());
			}
		}
		
	}
	if (Fruits.Num() <= 0)
	{
		//OwnerComp.GetBlackboardComponent()->SetValueAsVector("ClosestFruit", FVector::ZeroVector);
	}
}

void UBTS_UpdatePlayerValues::GetAllFruitActors(UWorld* WorldContext, TArray<AFoodActor*>& OutFruitActors)
{
	if (!WorldContext) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, AFoodActor::StaticClass(), FoundActors);

	OutFruitActors.Empty();
	for (AActor* Actor : FoundActors)
	{
		AFoodActor* Food = Cast<AFoodActor>(Actor);
		if (Food)
		{
			OutFruitActors.Add(Food);
		}
	}
}
