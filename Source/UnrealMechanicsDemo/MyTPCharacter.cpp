#include "MyTPCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Math/RotationMatrix.h"

AMyTPCharacter::AMyTPCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Player bu pawn'u kontrol etsin (spawn olmasa bile "görünmüyor" hissini çözer)
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Third person karakter ayarlarý
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	// SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	// Kamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	// Kamera için: controller yaw -> karakteri döndürsün
	bUseControllerRotationYaw = true;

	// CharacterMovement: artýk hareket yönüne göre dönmesin (controller'a göre dönsün)
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// (Varsa) controller pitch/roll kapalý kalsýn
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

void AMyTPCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyTPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyTPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyTPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMyTPCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyTPCharacter::LookUp);

	// Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AMyTPCharacter::MoveForward(float Value)
{
	if (!Controller || Value == 0.f) return;

	const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDir, Value);
}

void AMyTPCharacter::MoveRight(float Value)
{
	if (!Controller || Value == 0.f) return;

	const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector RightDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDir, Value);
}

void AMyTPCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMyTPCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}


