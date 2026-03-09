// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodActor.h"

#include "GameModePlay.h"
#include "Player2Movement.h"
#include "PlayerMovement.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFoodActor::AFoodActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

void AFoodActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFoodActor::OnSnakeCollision);

	APlayerMovement* Player = Cast<APlayerMovement>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player)
	{
		OnFoodEaten.AddDynamic(Player, &APlayerMovement::HandelFoodPickUp);
	}

	APlayer2Movement* Player2 = Cast<APlayer2Movement>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
	if (Player2)
	{
		OnFoodEaten.AddDynamic(Player2, &APlayer2Movement::HandelFoodPickUp2);
	}
	
}

void AFoodActor::OnSnakeCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpuls ,const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("HIT HIT HIT"));

	if (OtherActor->IsA(APlayerMovement::StaticClass()))
	{
		APlayerMovement* Player = Cast<APlayerMovement>(OtherActor);
		if (Player)
		{
			Player->HandelFoodPickUp(this); 
			AGameModePlay* GM = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				GM->AddScore(1, 1); 
			}
		}
	}
	else if (OtherActor->IsA(APlayer2Movement::StaticClass()))
	{
		APlayer2Movement* Player2 = Cast<APlayer2Movement>(OtherActor);
		if (Player2)
		{
			Player2->HandelFoodPickUp2(this);
			AGameModePlay* GM = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				GM->AddScore(1, 2); 
			}
		}
	}
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		PickUpEffect,
		GetActorLocation()
		);

	if (EatAppleSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EatAppleSound,
			GetActorLocation()
		);
	}
	
	Destroy();
}

