// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2Movement.h"
#include "FoodActor.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlayer2Movement::APlayer2Movement()
{
	// Create the Sphere Collision Component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	// Set it as the Root Component
	RootComponent = CollisionComponent;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(CollisionComponent);

	// Set collision settings
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->SetGenerateOverlapEvents(true);

	
	PlayerIndex = 1;

}

void APlayer2Movement::BeginPlay()
{
	Super::BeginPlay();

	
	
	CollisionComponent->OnComponentHit.AddDynamic(this, &APlayer2Movement::OnPlayerHit);
	
	TArray<AActor*> FoundActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFoodActor::StaticClass(), FoundActors);

	UE_LOG(LogTemp, Warning, TEXT("FoundActors size: %u"), FoundActors.Num());
	
	for (AActor* Actor : FoundActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("yes"));
		AFoodActor* Food = Cast<AFoodActor>(Actor);
		if (Food)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found food: %s"), *Food->GetName());
			Food->OnFoodEaten.AddDynamic(this, &APlayer2Movement::HandelFoodPickUp2);
		}
	}
	
	CurrentDirection = FVector(100, 0, 0);

	FVector InitialPosition = GetActorLocation();
	
	SegmentPositions.SetNum(50);
	for (int i = 0; i < SegmentPositions.Num(); i++)
	{
		SegmentPositions[i] = InitialPosition;
	}

	
	bIsMoving = false;
	StartNextStep();


}

// Called every frame
void APlayer2Movement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsMoving)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);


		if (bInserting)
		{
			if (SegmentInsertCount >= MaxSegments)
			{
				SegmentInsertCount = 0;
				
			}

			if (MoveStepCounter < StepsPerInsert)
			{
				MoveStepCounter++;
			}
			else
			{

				SetBodySegmentPosition(SegmentPositions, CurrentLocation);
				

				SegmentInsertCount++;
				MoveStepCounter = 0; // Reset to start movement again
			}
		}
		
		bool bMoved = SetActorLocation(NewLocation, true);
		

		if (FVector::DistSquared(NewLocation, TargetLocation) < 1.f)
		{
			SetActorLocation(TargetLocation, true); 
			bIsMoving = false;
			StartNextStep(); 
		}
	}

	

	for (int i = 0; i < SnakeSegments.Num(); i++)
	{
		if (SnakeSegments[i] && SegmentPositions.IsValidIndex(i))
		{
			FVector Cloc = SnakeSegments[i]->GetActorLocation();
			FVector Tloc = SegmentPositions[i];

			FVector NewLocation = FMath::VInterpTo(Cloc, Tloc, DeltaTime, SmoothSpeed);
			SnakeSegments[i]->SetActorLocation(NewLocation);
		}
	}
	
}


void APlayer2Movement::HandelFoodPickUp2(AActor* FoodActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Player 2 tried picking up"));

	UE_LOG(LogTemp, Warning, TEXT("PickUpFunction"));

	FVector SpawnLocation = SegmentPositions[++SegmentCount];

	AddBodySegment(SpawnLocation);
}

void APlayer2Movement::AddBodySegment(FVector SegmentStartPosition)
{
	
	UE_LOG(LogTemp, Warning, TEXT("NOM NOM NOM NO MN OM N O M NO M NO NM"));
	
	if (!SegmentClass) return; // Ensure it's set in Blueprint

	ASnakeSegment* NewSegment = GetWorld()->SpawnActor<ASnakeSegment>(
		SegmentClass,
		SegmentStartPosition,
		FRotator::ZeroRotator
	);

	if (NewSegment)
	{
		SegmentsOnSnake++;
		OnSegmentsUpdated.Broadcast(SegmentsOnSnake);
		SnakeSegments.Add(NewSegment);
		UE_LOG(LogTemp, Warning, TEXT("Added body segment at: %s"), *SegmentStartPosition.ToString());

		for (int i = 0; i < SnakeSegments.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("segment number: %d"), i);
		}
	}

}

void APlayer2Movement::SetBodySegmentPosition(TArray<FVector>& ArrayA, FVector Position)
{
	TArray<FVector> ArrayB;
	ArrayB.SetNumUninitialized(ArrayA.Num());

	ArrayB[0] = Position;
	
	for (int i = 1; i < ArrayA.Num(); i++)
	{
		ArrayB[i] = ArrayA[i - 1];
	}

	ArrayA = ArrayB;

	for (int i = 0; i < ArrayA.Num(); i++)
	{
	}
}

void APlayer2Movement::OnPlayerHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->ActorHasTag("Wall")) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit a wall!"));

		OnPlayer2Death.Broadcast();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		DeathEffect,
		GetActorLocation()
	);
	}
	if (OtherActor && OtherActor->ActorHasTag("Body")) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit own body!!"));

		OnPlayer2Death.Broadcast();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		DeathEffect,
		GetActorLocation()
		);
	}
}


void APlayer2Movement::StartNextStep()
{
	TargetLocation = GetActorLocation() + CurrentDirection.GetSafeNormal() * StepSize;
	bIsMoving = true;
}

void APlayer2Movement::SetDirectionExternal(const FVector& NewDir)
{
    // Check if NewDir is opposite of CurrentDirection to prevent 180 turn
    if (!NewDir.Equals(-CurrentDirection, 0.1f))
    {
        CurrentDirection = NewDir;
    }
}

void APlayer2Movement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

		
		// Player 2 (WASD)
	PlayerInputComponent->BindAction("P2_Up", IE_Pressed, this, &APlayer2Movement::MoveUp);
	PlayerInputComponent->BindAction("P2_Down", IE_Pressed, this, &APlayer2Movement::MoveDown);
	PlayerInputComponent->BindAction("P2_Left", IE_Pressed, this, &APlayer2Movement::MoveLeft);
	PlayerInputComponent->BindAction("P2_Right", IE_Pressed, this, &APlayer2Movement::MoveRight);
	UE_LOG(LogTemp, Warning, TEXT("Binding input for Player %d"), PlayerIndex);
	
}

void APlayer2Movement::MoveUp()
{
	if (CurrentDirection != FVector(0, -1, 0)) CurrentDirection = FVector(0, 1, 0);
	UE_LOG(LogTemp, Warning, TEXT("MOVE UP from PlayerIndex=%d"), PlayerIndex);
}

void APlayer2Movement::MoveDown()
{
	if (CurrentDirection != FVector(0, 1, 0)) CurrentDirection = FVector(0, -1, 0);
	UE_LOG(LogTemp, Warning, TEXT("input on player 2 pressed"));
}

void APlayer2Movement::MoveLeft()
{
	if (CurrentDirection != FVector(-1, 0, 0)) CurrentDirection = FVector(1, 0, 0);
	UE_LOG(LogTemp, Warning, TEXT("input on player 2 pressed"));
}

void APlayer2Movement::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("input on player 2 pressed"));
	if (CurrentDirection != FVector(1, 0, 0)) CurrentDirection = FVector(-1, 0, 0);
}
