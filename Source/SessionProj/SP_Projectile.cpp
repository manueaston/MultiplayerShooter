// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "SessionProjCharacter.h"
#include "SP_PlayerState.h"

// Sets default values
ASP_Projectile::ASP_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	//Create the sphere collision and make it the root component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
	RootComponent = SphereCollision;

	//Create the projectile mesh, turn off its collision, and attach it to the sphere collision
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASP_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
	if (ASessionProjCharacter* OwnerCharacter = Cast<ASessionProjCharacter>(GetOwner()))
	{
		OwnerCharacter->MoveIgnoreActorAdd(this);

		//Set the playerstate here
		OwnerPlayerState = Cast<ASP_PlayerState>(OwnerCharacter->GetPlayerState());
	}
}

void ASP_Projectile::OnProjectileHit(UPrimitiveComponent* ThisHitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	OnHitParticle();

	if (HasAuthority())
	{
		if (ASessionProjCharacter* HitCharacter = Cast<ASessionProjCharacter>(OtherActor))
		{
			FVector ImpulseDirection = GetVelocity();
			ImpulseDirection.Normalize();

			HitCharacter->SP_TakeDamage(Damage, OwnerPlayerState, ImpulseDirection);
		}

		Destroy();
	}
}

// Called every frame
void ASP_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

