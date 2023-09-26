// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SP_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONPROJ_API ASP_GameMode : public AGameMode
{
	GENERATED_BODY()

public:

	void RespawnPlayer(APlayerController* _PlayerToRespawn);
	
	//Checks for a winner based on eliminations
	void CheckForWinner();

protected:

	//Called when state enters WaitingPostMatch
	virtual void HandleMatchHasEnded() override;

	FTimerHandle MatchRestartDelayTimer;
};
