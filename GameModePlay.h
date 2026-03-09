// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModePlay.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API AGameModePlay : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ShowDeathScreen();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DeathScreenWidgetClass;


	UPROPERTY(EditAnywhere, Category="Players")
	TSubclassOf<APawn> Player2SnakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	AActor* SharedCameraActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	APawn* Player1Pawn;
	
	APawn* Player2Pawn;

	
	
	UFUNCTION(BlueprintCallable)
	void RestartSnakeLevel();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTwoPlayerMode = false;

	UPROPERTY(BlueprintReadOnly)
	int32 Player1Score = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 Player2Score = 0;

	
	
	UFUNCTION(BlueprintCallable)
	void AddScore(int32 Amount, int32 PlayerID);
};
