// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoidCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "Components/VoidCharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Public/Weapons/Sword/Blade.h"


//////////////////////////////////////////////////////////////////////////
// AVoidCharacter

AVoidCharacter::AVoidCharacter() :
CharacterState(ECharacterState::ECS_Idle),
bChangeState(true),
DashTime(1.0f),
bCanDash(true),
DashCooldown(2.0f),
DashThreshold(10.0f)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	NcDashLine = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Dash Line Component"));
	NcDashLine->SetupAttachment(FollowCamera);
	NcDashLine->SetAutoActivate(false);

	VoidCharacterMovementComponent = CreateDefaultSubobject<UVoidCharacterMovementComponent>("Void Character Movement Component");
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AVoidCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AVoidCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AVoidCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AVoidCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AVoidCharacter::StopJump);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVoidCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVoidCharacter::Look);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AVoidCharacter::Dash);

		EnhancedInputComponent->BindAction(EquipSwordAction, ETriggerEvent::Completed, this, &AVoidCharacter::HolsterSword);

	}

}

void AVoidCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if(bChangeState)
			CharacterState = ECharacterState::ECS_Running;
	}
}

void AVoidCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AVoidCharacter::Jump()
{
	ACharacter::Jump();

	//CharacterState = ECharacterState::ECS_Jumping;
}

void AVoidCharacter::StopJump()
{
	ACharacter::StopJumping();
}

void AVoidCharacter::Dash()
{
	VoidCharacterMovementComponent->InitiateDash();
}

void AVoidCharacter::HolsterSword()
{
	if(bSwordEquip)
	{
		bSwordEquip = false;
		Blade->Destroy();
		Blade = nullptr;
	}
	else
	{
		if(!Blade)
			Blade = SpawnSword();
		
		EquipSword(Blade);
		bSwordEquip = true;	
	}
}

void AVoidCharacter::EquipSword(ABlade* bladeToEquip)
{
	if(bladeToEquip)
	{
		const USkeletalMeshSocket* SkeletalMeshSocket = GetMesh()->GetSocketByName("SwordSocket");
		if(SkeletalMeshSocket)
		{
			SkeletalMeshSocket->AttachActor(bladeToEquip, GetMesh());
		}
	}
}

ABlade* AVoidCharacter::SpawnSword()
{
	if(BladeClass)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		return GetWorld()->SpawnActor<ABlade>(BladeClass, SpawnParameters);
	}
	return nullptr;
}


