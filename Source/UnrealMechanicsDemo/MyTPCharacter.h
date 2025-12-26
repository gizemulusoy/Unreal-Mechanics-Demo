#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyTPCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UNREALMECHANICSDEMO_API AMyTPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyTPCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Kamera
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;

	// Input fonksiyonlarý (klasik input)
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
};
