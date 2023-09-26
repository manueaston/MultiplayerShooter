// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_PickupSpawner.h"
#include "SP_Pickup.h"
#include "Components/SceneComponent.h"

// Sets default values
ASP_PickupSpawner::ASP_PickupSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	RootComponent = RootSceneComponent;

	SpawnLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Scene Component"));
	SpawnLocationComponent->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASP_PickupSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SpawnPickup();
	}
}

void ASP_PickupSpawner::SpawnPickup()
{
	if (PickupClass)
	{
		FTransform SpawnTransform = SpawnLocationComponent->GetComponentTransform();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		SpawnedPickup = GetWorld()->SpawnActor<ASP_Pickup>(PickupClass, SpawnTransform, SpawnParams);

		if (SpawnedPickup)
		{
			// Bind start respawn timer to destroy event
			SpawnedPickup->OnDestroyed.AddDynamic(this, &ASP_PickupSpawner::StartRespawnTimer);
		}
	}
}

void ASP_PickupSpawner::StartRespawnTimer(AActor* DestroyedActor)
{
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASP_PickupSpawner::SpawnPickup, RespawnTime, false);
}