// Copyright Epic Games, Inc. All Rights Reserved.

#include "SessionProjCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Net/UnrealNetwork.h"
#include "SP_Projectile.h"
#include "SP_PlayerState.h"
#include "SP_PlayerController.h"


//////////////////////////////////////////////////////////////////////////
// ASessionProjCharacter

ASessionProjCharacter::ASessionProjCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASessionProjCharacter::BeginPlay()
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

void ASessionProjCharacter::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	if (HasAuthority())
	{
		ControlPitch = GetControlRotation().Pitch;
	}
}

float ASessionProjCharacter::GetReplicatedPitch()
{
	if (IsLocallyControlled())
	{
		return GetControlRotation().Pitch;
	}

	return ControlPitch;
}

void ASessionProjCharacter::SP_TakeDamage(float _Damage, class ASP_PlayerState* _PlayerThatDealtDamage, FVector HitDirection)
{
	Health -= _Damage;

	if (Health <= 0)
	{
		Die(_PlayerThatDealtDamage, HitDirection);
	}
}

void ASessionProjCharacter::Die(ASP_PlayerState* _PlayerThatEliminatedYou, FVector HitDirection)
{
	_PlayerThatEliminatedYou->GiveElimination();

	if (ASP_PlayerController* PlayerController = Cast<ASP_PlayerController>(GetController()))
	{
		PlayerController->RespawnAfterDelay();
	}

	MulticastRagdoll(HitDirection);

	GetWorldTimerManager().SetTimer(DeathTimer, this, &ThisClass::DestroyAfterDelay, 3.0f, false);
}

void ASessionProjCharacter::DestroyAfterDelay()
{
	Destroy();
}

void ASessionProjCharacter::MulticastRagdoll_Implementation(FVector HitDirection)
{
	if (ASP_PlayerController* PlayerController = Cast<ASP_PlayerController>(GetController()))
	{
		PlayerController->DisableInput(PlayerController);
	}

	GetCharacterMovement()->DisableMovement();

	OnRagdoll(HitDirection);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASessionProjCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASessionProjCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASessionProjCharacter::Look);

		//Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASessionProjCharacter::Attack);
	}

}

void ASessionProjCharacter::Attack(const FInputActionValue& Value)
{
	ServerAttack();
}

void ASessionProjCharacter::ServerAttack_Implementation()
{
	if (Ammo > 0)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetMesh()->GetBoneLocation("hand_l"));
		SpawnTransform.SetRotation(GetControlRotation().Quaternion());

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		ASP_Projectile* SpawnProjectile = GetWorld()->SpawnActor<ASP_Projectile>(ProjectileClass, SpawnTransform, SpawnParams);

		Ammo--;

		MulticastFireSound();
	}
}

bool ASessionProjCharacter::ServerAttack_Validate()
{
	return true;
}

void ASessionProjCharacter::MulticastFireSound_Implementation()
{
	OnPlayFireSound();
}

void ASessionProjCharacter::Move(const FInputActionValue& Value)
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
	}
}

void ASessionProjCharacter::Look(const FInputActionValue& Value)
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

// Returns health as a number
float ASessionProjCharacter::GetHealth()
{
	return Health;
}

// Gives health up to 100 total
void ASessionProjCharacter::GiveHealth(float HealthToGive)
{
	Health = FMath::Clamp(Health + HealthToGive, 0, 100);
}

// Returns health as a normalized percentage
float ASessionProjCharacter::GetNormalizedHealth()
{
	return Health / 100.0f;
}

int ASessionProjCharacter::GetAmmo()
{
	return Ammo;
}

// Gives ammo, no cap
void ASessionProjCharacter::GiveAmmo(int AmmoToGive)
{
	Ammo += AmmoToGive;
}

void ASessionProjCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASessionProjCharacter, ControlPitch);
	DOREPLIFETIME(ASessionProjCharacter, Health);
	DOREPLIFETIME(ASessionProjCharacter, Ammo);
}


