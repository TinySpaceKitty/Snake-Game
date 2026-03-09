#include "LocalSnakeController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "PlayerMovement.h"
#include "Player2Movement.h"

void ALocalSnakeController::BeginPlay()
{
    Super::BeginPlay();

    Player1Pawn = GetPawn();

    GetWorldTimerManager().SetTimer(CameraTimerHandle, this, &ALocalSnakeController::ForceSharedCamera, 0.05f, false);
}

void ALocalSnakeController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("MoveUp", IE_Pressed, this, &ALocalSnakeController::P1_MoveUp);
    InputComponent->BindAction("MoveDown", IE_Pressed, this, &ALocalSnakeController::P1_MoveDown);
    InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ALocalSnakeController::P1_MoveLeft);
    InputComponent->BindAction("MoveRight", IE_Pressed, this, &ALocalSnakeController::P1_MoveRight);

    InputComponent->BindAction("P2_Up", IE_Pressed, this, &ALocalSnakeController::P2_MoveUp);
    InputComponent->BindAction("P2_Down", IE_Pressed, this, &ALocalSnakeController::P2_MoveDown);
    InputComponent->BindAction("P2_Left", IE_Pressed, this, &ALocalSnakeController::P2_MoveLeft);
    InputComponent->BindAction("P2_Right", IE_Pressed, this, &ALocalSnakeController::P2_MoveRight);
}

void ALocalSnakeController::RegisterPlayer2Pawn(APawn* InP2Pawn)
{
    Player2Pawn = InP2Pawn;
}

static void SafeSetDirection(APawn* Pawn, const FVector& Dir)
{
    if (!Pawn) return;
    if (APlayerMovement* P1 = Cast<APlayerMovement>(Pawn))
    {
        P1->SetDirectionExternal(Dir);
    }
    else if (APlayer2Movement* P2 = Cast<APlayer2Movement>(Pawn))
    {
        P2->SetDirectionExternal(Dir);
    }
}

// P1 handlers (arrow keys) — forward to possessed pawn
void ALocalSnakeController::P1_MoveUp()    { SafeSetDirection(Player1Pawn, FVector(0,1,0)); UE_LOG(LogTemp, Warning, TEXT("Binding input for Player p1"));}
void ALocalSnakeController::P1_MoveDown()  { SafeSetDirection(Player1Pawn, FVector(0,-1,0)); }
void ALocalSnakeController::P1_MoveLeft()  { SafeSetDirection(Player1Pawn, FVector(1, 0, 0)); }
void ALocalSnakeController::P1_MoveRight() { SafeSetDirection(Player1Pawn, FVector(-1, 0, 0)); }

// P2 handlers (WASD) — forward to unpossessed pawn reference
void ALocalSnakeController::P2_MoveUp()    { SafeSetDirection(Player2Pawn, FVector(0,1,0)); UE_LOG(LogTemp, Warning, TEXT("Binding input for Player p2"));}
void ALocalSnakeController::P2_MoveDown()  { SafeSetDirection(Player2Pawn, FVector(0,-1,0)); }
void ALocalSnakeController::P2_MoveLeft()  { SafeSetDirection(Player2Pawn, FVector(1, 0, 0)); }
void ALocalSnakeController::P2_MoveRight() { SafeSetDirection(Player2Pawn, FVector(-1, 0, 0)); }

void ALocalSnakeController::ForceSharedCamera()
{
    if (!SharedCameraActor) return;
    SetViewTarget(SharedCameraActor);
}