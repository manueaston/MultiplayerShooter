// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SP_PickupSpawner.generated.h"

UCLASS()
class SESSIONPROJ_API ASP_PickupSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASP_PickupSpawner();

protected:

	// Functions

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnPickup();

	// Called when pickup is destroyed
	UFUNCTION()
		void StartRespawnTimer(AActor* DestroyedActor);


	// Properties

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* RootSceneComponent = nullptr;

	// location where pickup will spawn
	UPROPERTY(EditDefaultsOnly)
		class USceneComponent* SpawnLocationComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Spawner)
		float RespawnTime = 5.0f;

	// Class of pickup that spawner will spawn
	UPROPERTY(EditDefaultsOnly, Category = Spawner)
		TSubclassOf<class ASP_Pickup> PickupClass = nullptr;

	// Pickup spawned
	ASP_Pickup* SpawnedPickup = nullptr;

	// Timer for respawning pickup
	FTimerHandle RespawnTimerHandle;

};
