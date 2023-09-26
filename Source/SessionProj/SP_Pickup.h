// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SP_Pickup.generated.h"

UCLASS()
class SESSIONPROJ_API ASP_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASP_Pickup();

protected:
	
	// Properties

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* SphereCollision = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		float BobHeight = 20;

	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		float BobSpeed = 200;

	float RandomBobStart = 0.0f;

	// Functions

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPickedUp(class ASessionProjCharacter* Character) {};

	UFUNCTION(BlueprintImplementableEvent)
		void BPOnPickedUp(ASessionProjCharacter* Character);

	// Overlap function
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* ThisHitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
