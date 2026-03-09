// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeSegment.h"

// Sets default values
ASnakeSegment::ASnakeSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Sphere Collision Component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	// Set it as the Root Component
	RootComponent = CollisionComponent;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(CollisionComponent);

	// Set collision settings
	CollisionComponent->SetCollisionProfileName(TEXT("Actor"));
	CollisionComponent->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void ASnakeSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

