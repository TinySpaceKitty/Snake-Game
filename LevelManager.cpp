// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMovement.h"

ALevelManager::ALevelManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	APlayerMovement* Player = Cast<APlayerMovement>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player)
	{
		Player->OnSegmentsUpdated.AddDynamic(this, &ALevelManager::OnSegmentsUpdated);
	}
}

void ALevelManager::OnSegmentsUpdated(int32 NewSegmentCount)
{

	if (NewSegmentCount >= RequiredSegments)
	{
		ShowLevelCompleteWidget();

		UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
		if (GI)
		{
			GI->SetState(ESnakeGameState::GameOver);
		}
	}
}

void ALevelManager::ShowLevelCompleteWidget()
{
	if (LevelCompleteWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), LevelCompleteWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

