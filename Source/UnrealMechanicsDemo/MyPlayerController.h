#pragma once

#include "CoreMinimal.h"
#include "Misc/Optional.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class UNREALMECHANICSDEMO_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};
