#pragma once
#include "UObject/ObjectMacros.h"
#include "ESnakeGameState.generated.h"

UENUM(BlueprintType)
enum class ESnakeGameState : uint8
{
	MainMenu UMETA(DisplayName = "Main Menu"),
	SoloGame UMETA(DisplayName = "Solo Game"),
	AIGame UMETA(DisplayName = "AI Game"),
	GameOver UMETA(DisplayName = "Game Over")
};
