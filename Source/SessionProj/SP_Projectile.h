// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SP_Projectile.generated.h"

UCLASS()
class SESSIONPROJ_API ASP_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASP_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The projectile movement component
	UPROPERTY(EditDefaultsOnly)
		class UProjectileMovementComponent* ProjectileMovement = nullptr;

	//Static mesh component
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* ProjectileMesh = nullptr;

	//Sphere collision component
	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* SphereCollision = nullptr;

	//Called when the projectile hits something
	UFUNCTION()
		void OnProjectileHit(UPrimitiveComponent* ThisHitComp, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	//The damage that this projectile deals
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		float Damage = 20.0f;

	//The owning playerstate
	class ASP_PlayerState* OwnerPlayerState = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
		void OnHitParticle();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
