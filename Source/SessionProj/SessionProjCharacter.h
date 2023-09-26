// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SessionProjCharacter.generated.h"


UCLASS(config=Game)
class ASessionProjCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AttackAction;

public:
	ASessionProjCharacter();
	
	virtual void Tick(float Deltatime) override;

	UFUNCTION(BlueprintPure)
		float GetReplicatedPitch();

	// Returns health as a number
	UFUNCTION(BlueprintPure)
		float GetHealth();

	// Gives health, cap of 100
	UFUNCTION(BlueprintCallable)
		void GiveHealth(float HealthToGive);

	// Gives ammo, no cap
	UFUNCTION(BlueprintCallable)
		void GiveAmmo(int AmmoToGive);

	// Returns health as a normalized percentage
	UFUNCTION(BlueprintPure)
		float GetNormalizedHealth();

	UFUNCTION(BlueprintPure)
		int GetAmmo();

	//Called when hit by a projectile. Should be called on the server
	void SP_TakeDamage(float _Damage, class ASP_PlayerState* _PlayerThatDealtDamage, FVector HitDirection);

	//Called when you run out of health
	void Die(ASP_PlayerState* _PlayerThatEliminatedYou, FVector HitDirection);

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for looking input */
	void Attack(const FInputActionValue& Value);

	//Attempt to fire a projectile on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttack();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastFireSound();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPlayFireSound();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRagdoll(FVector HitDirection);

	UFUNCTION(BlueprintImplementableEvent)
		void OnRagdoll(FVector HitDirection);

	void DestroyAfterDelay();
	FTimerHandle DeathTimer;

	UPROPERTY(Replicated)
		float ControlPitch = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASP_Projectile> ProjectileClass = nullptr;

	UPROPERTY(Replicated)
		float Health = 100.0f;

	UPROPERTY(Replicated)
		int Ammo = 20;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

