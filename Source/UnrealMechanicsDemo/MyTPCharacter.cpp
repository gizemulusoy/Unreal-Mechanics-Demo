#include "MyTPCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Math/RotationMatrix.h"

#include "GameFramework/Character.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

#include "Engine/Engine.h"



AMyTPCharacter::AMyTPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Player bu pawn'u kontrol etsin (spawn olmasa bile "görünmüyor" hissini çözer)
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Third person settings
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

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	// Kamera için: controller yaw -> karakteri döndürsün
	//bUseControllerRotationYaw = true;

	// CharacterMovement: artýk hareket yönüne göre dönmesin (controller'a göre dönsün)
	//GetCharacterMovement()->bOrientRotationToMovement = false;

	// (Varsa) controller pitch/roll kapalý kalsýn
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;



	// Skeletal Mesh ayarý
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple")
	);

	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}

	// AnimBP çalýþmasý için þart
	GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C")
	);

	if (AnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	}


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
	//PlayerInputComponent->BindAxis("MoveRight", this, &AMyTPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMyTPCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyTPCharacter::LookUp);

	// Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyTPCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyTPCharacter::StopSprint);

	// Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyTPCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyTPCharacter::StopCrouch);

	
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyTPCharacter::MoveRight);


	//  Backpedal
	PlayerInputComponent->BindAction("Backpedal", IE_Pressed, this, &AMyTPCharacter::BackpedalPressed);
	PlayerInputComponent->BindAction("Backpedal", IE_Released, this, &AMyTPCharacter::BackpedalReleased);



	


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

// Sprint : 

void AMyTPCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 900.f;


	// Debug check: sprint functionality
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 9999.f, FColor::Green, TEXT("SPRINT ON (Speed=900)"));
}

void AMyTPCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	if (GEngine)
		GEngine->RemoveOnScreenDebugMessage(1);
}

// Crouch : 

void AMyTPCharacter::StartCrouch()
{
	Crouch();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(2, 9999.f, FColor::Green, TEXT("Crouch : ON "));
}

void AMyTPCharacter::StopCrouch()
{
	UnCrouch();

	if (GEngine)
		GEngine->RemoveOnScreenDebugMessage(2);
}

void AMyTPCharacter::BackpedalPressed()
{
	// S basýlýyken: STRAFE / BACKPEDAL modu
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AMyTPCharacter::BackpedalReleased()
{
	// S býrakýlýnca: NORMAL third-person
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}






