// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ESnakeGameState.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */


UCLASS()
class SNAKEGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// Current game state
	UPROPERTY(BlueprintReadOnly)
	ESnakeGameState CurrentState = ESnakeGameState::MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> SoloLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> AILevels;

	UPROPERTY(BlueprintReadWrite)
	bool bPlayAgainstAI = false;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentLevelIndex = 0;

	UFUNCTION(BlueprintCallable)
	void SetState(ESnakeGameState NewState);

	UFUNCTION(BlueprintCallable)
	void StartSoloGame();

	UFUNCTION(BlueprintCallable)
	void StartAIGame();

	UFUNCTION(BlueprintCallable)
	void LoadNextLevel();

	UFUNCTION(BlueprintCallable)
	void ReturnToMainMenu();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bForceTwoPlayers = false;

	UFUNCTION(BlueprintCallable)
	void StartSoloWithTwoPlayers();

	UFUNCTION(BlueprintCallable)
	void StartSoloSinglePlayer();

private:
	void HandleStateEnter(ESnakeGameState NewState);
};
