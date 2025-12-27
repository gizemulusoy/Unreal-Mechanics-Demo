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
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* CharacterMesh;


	// klasik input
	void MoveForward(float Value);

	void MoveRight(float Value);

	void Turn(float Value);
	void LookUp(float Value);

	void StartSprint();
	void StopSprint();

	void StartCrouch();
	void StopCrouch();

	

	



};
