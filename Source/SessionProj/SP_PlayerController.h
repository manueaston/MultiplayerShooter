// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONPROJ_API ASP_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	//Respawns after a delay.
	void RespawnAfterDelay();

protected:

	//Calls the GameMode to try to respawn
	void TryRespawn();

	//Handle Used for the timer to respawn
	FTimerHandle RespawnTimer;
	
};
