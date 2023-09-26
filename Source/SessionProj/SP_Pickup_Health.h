// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SP_Pickup.h"
#include "SP_Pickup_Health.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONPROJ_API ASP_Pickup_Health : public ASP_Pickup
{
	GENERATED_BODY()

protected:

	virtual void OnPickedUp(class ASessionProjCharacter* Character) override;

	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		float HealthToGive = 30.0f;
};
