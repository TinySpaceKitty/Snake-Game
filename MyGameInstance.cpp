


#include "MyGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UMyGameInstance::HandleStateEnter(ESnakeGameState NewState)
{
	switch (NewState)
	{
	case ESnakeGameState::MainMenu:
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
		break;

	case ESnakeGameState::SoloGame:
		{
			FString LevelName = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
			UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
			break;
		}

	case ESnakeGameState::AIGame:
		{
			FString LevelName = FString::Printf(TEXT("Level_%d_AI"), CurrentLevelIndex);
			UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
			break;
		}
	}
}

void UMyGameInstance::SetState(ESnakeGameState NewState)
{
	CurrentState = NewState;
	HandleStateEnter(NewState);
	UE_LOG(LogTemp, Warning, TEXT("Game State changed to %d"), (uint8)NewState);
}

void UMyGameInstance::StartSoloGame()
{
	bPlayAgainstAI = false;
	CurrentLevelIndex = 1;
	SetState(ESnakeGameState::SoloGame);

}

void UMyGameInstance::StartAIGame()
{
	bPlayAgainstAI = true;
	CurrentLevelIndex = 1;
	SetState(ESnakeGameState::AIGame);

}

void UMyGameInstance::LoadNextLevel()
{
	CurrentLevelIndex++;


	if (CurrentLevelIndex <= 3)
	{
		if (bPlayAgainstAI == true)
		{
			FString LevelName = FString::Printf(TEXT("Level_%d_AI"), CurrentLevelIndex);
			UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
		}

		if (bPlayAgainstAI == false)
		{
			FString LevelName = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
			UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
		}
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}
	}
	else
	{
		ReturnToMainMenu();
	}
}

void UMyGameInstance::ReturnToMainMenu()
{
	SetState(ESnakeGameState::MainMenu);
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu_Level");
}

void UMyGameInstance::StartSoloWithTwoPlayers()
{
	bForceTwoPlayers = true;
	bPlayAgainstAI = false;
	CurrentLevelIndex = 1;
	
	UGameplayStatics::OpenLevel(this, FName("Level_1"));
}

void UMyGameInstance::StartSoloSinglePlayer()
{
	bForceTwoPlayers = false;
	bPlayAgainstAI = false;
	CurrentLevelIndex = 1;
	UGameplayStatics::OpenLevel(this, FName("Level_1"));
}