// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SP_Pickup.h"
#include "SP_Pickup_Ammo.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONPROJ_API ASP_Pickup_Ammo : public ASP_Pickup
{
	GENERATED_BODY()
	
	virtual void OnPickedUp(class ASessionProjCharacter* Character) override;

	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		int AmmoToGive = 10;
};
