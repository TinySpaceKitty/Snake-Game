// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlay.h"
#include "LocalSnakeController.h"
#include "MyGameInstance.h"
#include "Player2Movement.h"
#include "PlayerMovement.h"
#include "GameFramework/PlayerController.h"

void AGameModePlay::BeginPlay()
{
	Super::BeginPlay();

UWorld* World = GetWorld();
    if (!World) return;

	UE_LOG(LogTemp, Error, TEXT("STILL WORKSSSSS"));
	
    ALocalSnakeController* LocalPC = Cast<ALocalSnakeController>(UGameplayStatics::GetPlayerController(World, 0));
    if (!LocalPC)
    {
        UE_LOG(LogTemp, Error, TEXT("LocalSnakeController not found as PC0."));
        return;
    }

    Player1Pawn  = LocalPC->GetPawn();
    if (!Player1Pawn)
    {
    	
        UE_LOG(LogTemp, Warning, TEXT("Player1 pawn not yet possessed — scheduling a short delay."));
    	
        FTimerHandle RetryHandle;
        GetWorldTimerManager().SetTimer(RetryHandle, [this, LocalPC]() {
            APawn* P1 = LocalPC->GetPawn();
            if (P1) LocalPC->Player1Pawn = P1; 
        }, 0.05f, false);
    }

	if (Player1Pawn)
	{
		APlayerMovement* P1Move = Cast<APlayerMovement>(Player1Pawn);
		if (P1Move)
		{
			P1Move->OnPlayerDeath.AddDynamic(this, &AGameModePlay::ShowDeathScreen);
			UE_LOG(LogTemp, Warning, TEXT("Bound Player1 OnPlayerDeath delegate."));
		}
	}

	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		if (GI->bForceTwoPlayers)
		{
			bTwoPlayerMode = true;
		}
	}
	
    // Spawn Player 2 (only if two-player)
    if (bTwoPlayerMode)
    {
        if (!Player2SnakeClass)
        {
            UE_LOG(LogTemp, Error, TEXT("Player2SnakeClass not set."));
        }
        else
        {
        	
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            FVector SpawnLoc(220.f, 0.f, 90.f); // pick safe spot
            APawn* P2 = World->SpawnActor<APawn>(Player2SnakeClass, SpawnLoc, FRotator::ZeroRotator, SpawnParams);
            if (P2)
            {
            	Player2Pawn = P2;
                // Register P2 with controller so it can forward input
                LocalPC->RegisterPlayer2Pawn(P2);
                UE_LOG(LogTemp, Warning, TEXT("Spawned Player2 pawn and registered with local controller."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn Player2 pawn"));
            }
        }

    		APlayer2Movement* P2Move = Cast<APlayer2Movement>(Player2Pawn);
    		if (P2Move)
    		{
    			P2Move->OnPlayer2Death.AddDynamic(this, &AGameModePlay::ShowDeathScreen);
    			UE_LOG(LogTemp, Warning, TEXT("Bound Player2 OnPlayerDeath delegate."));
    		}
    	
    }
	
	LocalPC->SetupInputComponent();

    LocalPC->SharedCameraActor = SharedCameraActor;
	
	
}

void AGameModePlay::ShowDeathScreen()
{
	if (DeathScreenWidgetClass)
	{
		UUserWidget* DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathScreenWidgetClass);
		if (DeathWidget)
		{
			DeathWidget->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}

		UFunction* SetScoreFunc = DeathWidget->FindFunction(TEXT("SetFinalScores"));
		if (SetScoreFunc)
		{
			struct FScoreParams
			{
				int32 P1;
				int32 P2;
			};

			FScoreParams Params;
			Params.P1 = Player1Score;
			Params.P2 = Player2Score;

			DeathWidget->ProcessEvent(SetScoreFunc, &Params);
		}
		
		ALocalSnakeController* PC = Cast<ALocalSnakeController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly()); 
		}
	}
}

void AGameModePlay::RestartSnakeLevel()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}

	FName CurrentLevel = FName(*GetWorld()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Restarting level: %s"), *CurrentLevel.ToString());

	UGameplayStatics::OpenLevel(this, CurrentLevel);
}

void AGameModePlay::AddScore(int32 Amount, int32 PlayerID)
{
	if (PlayerID == 1)
		Player1Score += Amount;
	else if (PlayerID == 2)
		Player2Score += Amount;

	UE_LOG(LogTemp, Warning, TEXT("Score updated: P1=%d  P2=%d"), Player1Score, Player2Score);
}
