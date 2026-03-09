#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LocalSnakeController.generated.h"

UCLASS()
class SNAKEGAME_API ALocalSnakeController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void RegisterPlayer2Pawn(APawn* InP2Pawn);

protected:
	void P1_MoveUp();
	void P1_MoveDown();
	void P1_MoveLeft();
	void P1_MoveRight();

	void P2_MoveUp();
	void P2_MoveDown();
	void P2_MoveLeft();
	void P2_MoveRight();

	// Force shared camera after small delay
	void ForceSharedCamera();

	FTimerHandle CameraTimerHandle;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	AActor* SharedCameraActor;

	
	// references
	APawn* Player1Pawn = nullptr; // possessed by this controller
	APawn* Player2Pawn = nullptr; // unpossessed, spawned by GameMode
};