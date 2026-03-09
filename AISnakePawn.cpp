// Fill out your copyright notice in the Description page of Project Settings.


#include "AISnakePawn.h"
#include "FoodActor.h"
#include "SnakeSegment.h"
#include "Kismet/GameplayStatics.h"

AAISnakePawn::AAISnakePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->SetGenerateOverlapEvents(true);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(CollisionComponent);
}

void AAISnakePawn::BeginPlay()
{
	Super::BeginPlay();
    
	CollisionComponent->OnComponentHit.AddDynamic(this, &AAISnakePawn::OnHit);

	CurrentDirection = FVector(100, 0, 0);
	bIsMoving = false;

	FVector InitialPosition = GetActorLocation();
	SegmentPositions.SetNum(20);
	for (int i = 0; i < SegmentPositions.Num(); i++)
	{
		SegmentPositions[i] = InitialPosition;
	}


	
	GetWorldTimerManager().SetTimer(MovementTimer, this, &AAISnakePawn::StartNextStep, 0.3f, true);
}

void AAISnakePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
		SetActorLocation(NewLocation, true);

		if (FVector::DistSquared(NewLocation, TargetLocation) < 1.0f)
		{
			SetActorLocation(TargetLocation);
			bIsMoving = false;
			StartNextStep(); // Trigger next movement
		}
	}

	// Smooth segment following
	for (int i = 0; i < SnakeSegments.Num(); i++)
	{
		if (SnakeSegments[i] && SegmentPositions.IsValidIndex(i))
		{
			FVector CurrentSegLoc = SnakeSegments[i]->GetActorLocation();
			FVector TargetSegLoc = SegmentPositions[i];

			FVector NewLoc = FMath::VInterpTo(CurrentSegLoc, TargetSegLoc, DeltaTime, 10.0f);
			SnakeSegments[i]->SetActorLocation(NewLoc);
		}
	}
}

void AAISnakePawn::StartNextStep()
{
	if (CurrentDirection.IsNearlyZero()) return;

	// WALL AVOIDANCE 
	if (IsWallAhead(CurrentDirection))
	{
		FVector Left = FVector(-CurrentDirection.Y, CurrentDirection.X, 0);  // 90° turn
		FVector Right = FVector(CurrentDirection.Y, -CurrentDirection.X, 0); // -90° turn

		if (!IsWallAhead(Left))
		{
			CurrentDirection = Left;
		}
		else if (!IsWallAhead(Right))
		{
			CurrentDirection = Right;
		}
	}

	FVector CurrentLocation = GetActorLocation();
	SetBodySegmentPosition(SegmentPositions, CurrentLocation);

	TargetLocation = CurrentLocation + CurrentDirection.GetSafeNormal() * StepSize;
	bIsMoving = true;
}

void AAISnakePawn::SetBodySegmentPosition(TArray<FVector>& ArrayA, FVector Position)
{
	TArray<FVector> NewArray;
	NewArray.SetNumUninitialized(ArrayA.Num());
	NewArray[0] = Position;

	for (int i = 1; i < ArrayA.Num(); i++)
	{
		NewArray[i] = ArrayA[i - 1];
	}
	ArrayA = NewArray;
}

void AAISnakePawn::AddBodySegment(FVector SpawnLocation)
{
	if (!SegmentClass) return;

	ASnakeSegment* NewSegment = GetWorld()->SpawnActor<ASnakeSegment>(SegmentClass, SpawnLocation, FRotator::ZeroRotator);
	if (NewSegment)
	{
		SnakeSegments.Add(NewSegment);
	}
}

void AAISnakePawn::HandleFruitPickup()
{
	FVector SpawnLocation = SegmentPositions[++SegmentCount];
	AddBodySegment(SpawnLocation);
	
	
}

bool AAISnakePawn::IsWallAhead(const FVector& Direction)
{
	FVector CheckLocation = GetActorLocation() + Direction.GetSafeNormal() * StepSize;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		CheckLocation,
		CheckLocation,
		FQuat::Identity,
		ECC_WorldStatic,
		FCollisionShape::MakeSphere(40.0f),
		Params
	);

	return bHit && Hit.GetActor()->ActorHasTag("Wall");
}

void AAISnakePawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision happend"));
	if (OtherActor && (OtherActor->ActorHasTag("Wall") || OtherActor->ActorHasTag("Body")))
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Snake died"));
		MoveSpeed = 0;
	}
	else if (AFoodActor* Food = Cast<AFoodActor>(OtherActor))
	{
		HandleFruitPickup(); 
	}
}

void AAISnakePawn::SetDirectionTowardsX(float FruitX)
{
	FVector MyLocation = GetActorLocation();

	if (FMath::IsNearlyEqual(FruitX, MyLocation.X, 1.0f))
	{
		// Already aligned in X
		CurrentDirection = FVector::ZeroVector;
	}
	else if (FruitX > MyLocation.X)
	{
		CurrentDirection = FVector::RightVector; // (1, 0, 0)
	}
	else
	{
		CurrentDirection = FVector::LeftVector; // (-1, 0, 0)
	}
}

void AAISnakePawn::SetDirectionTowardsY(float FruitY)
{
	FVector MyLocation = GetActorLocation();

	if (FMath::IsNearlyEqual(FruitY, MyLocation.Y, 1.0f))
	{
		//CurrentDirection = FVector::ZeroVector;
		return;
	}
	else if (FruitY > MyLocation.Y)
	{
		CurrentDirection = FVector(0, 1, 0);   // UP (Y+)
	}
	else
	{
		CurrentDirection = FVector(0, -1, 0);  // DOWN (Y-)
	}
}