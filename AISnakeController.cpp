// Fill out your copyright notice in the Description page of Project Settings.


#include "AISnakeController.h"
#include "Components/StaticMeshComponent.h"
#include "AISnakePawn.h" 
#include "GameFramework/Pawn.h"


void AAISnakeController::BeginPlay()
{
   Super::BeginPlay();
   
   AAISnakePawn* AIPlayer = Cast<AAISnakePawn>(GetPawn());
   if (AIPlayer && AIPlayer->BTAsset)
   {
   RunBehaviorTree(AIPlayer->BTAsset);
   }
}
