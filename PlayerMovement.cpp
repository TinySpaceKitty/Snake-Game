// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovement.h"
#include "FoodActor.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
APlayerMovement::APlayerMovement()
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

	PlayerIndex = 0;
	
	
}

// Called when the game starts or when spawned
void APlayerMovement::BeginPlay()
{
	Super::BeginPlay();

	
	CollisionComponent->OnComponentHit.AddDynamic(this, &APlayerMovement::OnPlayerHit);
	
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
			Food->OnFoodEaten.AddDynamic(this, &APlayerMovement::HandelFoodPickUp);
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
void APlayerMovement::Tick(float DeltaTime)
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


void APlayerMovement::HandelFoodPickUp(AActor* FoodActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Player 1 tried picking up"));
	

	UE_LOG(LogTemp, Warning, TEXT("PickUpFunction"));

	FVector SpawnLocation = SegmentPositions[++SegmentCount];

	AddBodySegment(SpawnLocation);
}

void APlayerMovement::AddBodySegment(FVector SegmentStartPosition)
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

void APlayerMovement::SetBodySegmentPosition(TArray<FVector>& ArrayA, FVector Position)
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

void APlayerMovement::OnPlayerHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->ActorHasTag("Wall")) // or check by class
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit a wall!"));

		OnPlayerDeath.Broadcast();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		DeathEffect,
		GetActorLocation()
	);
	}
	if (OtherActor && OtherActor->ActorHasTag("Body")) // or check by class
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit own body!!"));

		OnPlayerDeath.Broadcast();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		DeathEffect,
		GetActorLocation()
		);
	}
}


void APlayerMovement::StartNextStep()
{
	TargetLocation = GetActorLocation() + CurrentDirection.GetSafeNormal() * StepSize;
	bIsMoving = true;
}

void APlayerMovement::SetDirectionExternal(const FVector& NewDir)
{
	if (!NewDir.Equals(-CurrentDirection, 0.1f))
	{
		CurrentDirection = NewDir;
	}
}

void APlayerMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &APlayerMovement::MoveUp);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &APlayerMovement::MoveDown);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerMovement::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerMovement::MoveRight);
	UE_LOG(LogTemp, Warning, TEXT("Binding input for Player %d"), PlayerIndex);
}

void APlayerMovement::MoveUp()
{
	UE_LOG(LogTemp, Warning, TEXT("MOVE UP from PlayerIndex=%d"), PlayerIndex);
	if (CurrentDirection != FVector(0, -1, 0)) CurrentDirection = FVector(0, 1, 0);
}

void APlayerMovement::MoveDown()
{
	if (CurrentDirection != FVector(0, 1, 0)) CurrentDirection = FVector(0, -1, 0);
}

void APlayerMovement::MoveLeft()
{
	if (CurrentDirection != FVector(-1, 0, 0)) CurrentDirection = FVector(1, 0, 0);
}

void APlayerMovement::MoveRight()
{
	
	if (CurrentDirection != FVector(1, 0, 0)) CurrentDirection = FVector(-1, 0, 0);
}
