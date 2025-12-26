#include "MyPlayerController.h"
#include "GameFramework/InputSettings.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Mouse cursor kapalý
    bShowMouseCursor = false;

    // Input'u oyuna ver
    SetInputMode(FInputModeGameOnly());
}
