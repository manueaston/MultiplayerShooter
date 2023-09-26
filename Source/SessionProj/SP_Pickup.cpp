// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_Pickup.h"
#include "Components/SphereComponent.h"
#include "SessionProjCharacter.h"

// Sets default values
ASP_Pickup::ASP_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	Mesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASP_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
	// Check this is the server
	if (HasAuthority())
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	}

	RandomBobStart = FMath::FRand() * 10;
}

// Called every frame
void ASP_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float BobAmount = FMath::Sin(FMath::DegreesToRadians((GetGameTimeSinceCreation() + RandomBobStart) * BobSpeed)) * BobHeight;

	Mesh->SetRelativeLocation(FVector(Mesh->GetRelativeLocation().X, Mesh->GetRelativeLocation().Y, BobAmount));
}

void ASP_Pickup::OnOverlapBegin(UPrimitiveComponent* ThisHitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check that other actor is character
	if (ASessionProjCharacter* OverlappedCharacter = Cast<ASessionProjCharacter>(OtherActor))
	{
		OnPickedUp(OverlappedCharacter);
		BPOnPickedUp(OverlappedCharacter);

		Destroy();
	}
}